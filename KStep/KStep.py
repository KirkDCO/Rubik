import heapq
import json
import pandas as pd
import time
from tqdm import tqdm

########################################
# File paths
puzzle_info_path = '../puzzle_info.csv'
puzzles_path = '../puzzles.csv'

# Loading the data
puzzle_info_df = pd.read_csv(puzzle_info_path)
puzzles_df = pd.read_csv(puzzles_path)

########################################
# Parsing the initial_state and solution_state columns
# Converting the semicolon-separated string values into lists of colors
puzzles_df['parsed_initial_state'] = puzzles_df['initial_state'].apply(lambda x: x.split(';'))
puzzles_df['parsed_solution_state'] = puzzles_df['solution_state'].apply(lambda x: x.split(';'))

########################################
# Converting the string representation of allowed_moves to dictionary
puzzle_info_df['allowed_moves'] = puzzle_info_df['allowed_moves'].apply(lambda x: json.loads(x.replace("'", '"')))

########################################
def apply_move(state, move, inverse=False):
    """
    Apply a move or its inverse to the puzzle state.

    :param state: List of colors representing the current state of the puzzle.
    :param move: List representing the move as a permutation.
    :param inverse: Boolean indicating whether to apply the inverse of the move.
    :return: New state of the puzzle after applying the move.
    """
    if inverse:
        # Creating a dictionary to map the original positions to the new positions
        inverse_move = {v: k for k, v in enumerate(move)}
        return [state[inverse_move[i]] for i in range(len(state))]
    else:
        return [state[i] for i in move]

########################################
def heuristic(state, goal_state):
    """
    Heuristic function estimating the cost from the current state to the goal state.
    Here, we use the number of mismatched colors between the current state and the goal state.
    """
    return sum(s != g for s, g in zip(state, goal_state))


def a_star_search_with_timeout(initial_state, goal_state, allowed_moves, max_depth, timeout=300):
    """
    A* search algorithm with a timeout feature.

    :param initial_state: The starting state of the puzzle.
    :param goal_state: The target state to reach.
    :param allowed_moves: A dictionary of moves that can be applied to the state.
    :param max_depth: Limit path length
    :param timeout: The maximum time (in seconds) allowed for the search.

    :return: The shortest sequence of moves to solve the puzzle, or None if unsolved within timeout.
    """

    start_time = time.time()
    open_set = []  # Priority queue for states to explore
    heapq.heappush(open_set, (0, initial_state, []))  # Each entry: (priority, state, path taken)

    closed_set = set()  # Set to keep track of already explored states

    while open_set:
        if time.time() - start_time > timeout:
            return None  # Timeout check

        _, current_state, path = heapq.heappop(open_set)

        if len(path) > max_depth:
            continue

        if current_state == goal_state:
            return path  # Goal state reached

        state_tuple = tuple(current_state)
        if state_tuple in closed_set:
            continue  # Skip already explored states

        closed_set.add(state_tuple)

        for move_name, move in allowed_moves.items():
            for inverse in [False, True]:  # Consider both move and its inverse
                new_state = apply_move(current_state, move, inverse)
                new_state_tuple = tuple(new_state)
                if new_state_tuple not in closed_set:
                    priority = len(path) + 1 + heuristic(new_state, goal_state)
                    heapq.heappush(open_set, (priority, new_state, path + [(move_name, inverse)]))

########################################
# Modifying the A* search algorithm to improve efficiency
def improved_heuristic_with_wildcards(state, goal_state, num_wildcards):
    """
    Improved heuristic function considering wildcards.
    """
    mismatches = sum(s != g for s, g in zip(state, goal_state))
    return max(0, mismatches - num_wildcards)

def improved_a_star_search_with_wildcards(initial_state, goal_state, allowed_moves, num_wildcards, max_depth=30, timeout=100):
    """
    Improved A* search algorithm with wildcards, depth limit, and timeout.

    :param initial_state: List representing the initial state of the puzzle.
    :param goal_state: List representing the goal state of the puzzle.
    :param allowed_moves: Dictionary of allowed moves and their corresponding permutations.
    :param num_wildcards: Number of wildcards allowed for the puzzle.
    :param max_depth: Maximum depth to search to limit the search space.
    :param timeout: Time limit in seconds for the search.
    :return: Shortest sequence of moves to solve the puzzle, or None if no solution is found.
    """
    start_time = time.time()
    open_set = []
    heapq.heappush(open_set, (0, initial_state, [], num_wildcards))  # (priority, state, path, remaining wildcards)
    closed_set = set()

    while open_set:
        if time.time() - start_time > timeout:
            return None  # Timeout

        _, current_state, path, remaining_wildcards = heapq.heappop(open_set)

        if len(path) > max_depth:  # Depth limit
            continue

        if current_state == goal_state or improved_heuristic_with_wildcards(current_state, goal_state, remaining_wildcards) == 0:
            return path

        closed_set.add((tuple(current_state), remaining_wildcards))

        for move_name, move in allowed_moves.items():
            for inverse in [False, True]:
                new_state = apply_move(current_state, move, inverse)
                if (tuple(new_state), remaining_wildcards) not in closed_set:
                    priority = len(path) + 1 + improved_heuristic_with_wildcards(new_state, goal_state, remaining_wildcards)
                    heapq.heappush(open_set, (priority, new_state, path + [(move_name, inverse)], remaining_wildcards))

    return None  # No solution found

########################################
def format_solution_for_submission(puzzle_id, solution_moves):
    """
    Format the solution to a puzzle for submission.

    :param puzzle_id: The unique identifier of the puzzle.
    :param solution_moves: List of tuples representing the solution moves.
    :return: Formatted string suitable for submission.
    """
    formatted_moves = []
    for move, inverse in solution_moves:
        move_str = '-' + move if inverse else move
        formatted_moves.append(move_str)

    # Joining the moves into a single string separated by periods
    return {'id': puzzle_id, 'moves': '.'.join(formatted_moves)}

########################################
def solve_puzzles(puzzles_df, puzzle_info_df, target_submission_df, num_puzzles=None, limit_index=30):
    """
    Solve a set of puzzles using the A* search algorithm.

    :param puzzles_df: DataFrame containing puzzles.
    :param puzzle_info_df: DataFrame containing allowed moves for each puzzle type.
    :param target_submission_df: DataFrame containing sample submission format.
    :param num_puzzles: Number of puzzles to solve (if None, solve all).
    :return: DataFrame with the solutions formatted for submission.
    """
    solutions = []

    # Limit the number of puzzles if specified
    puzzles_to_solve = puzzles_df if num_puzzles is None else puzzles_df.head(num_puzzles)

    for index, row in tqdm(puzzles_to_solve.iterrows(), total=puzzles_to_solve.shape[0], desc="Solving Puzzles"):
        puzzle_id = row['id']
        initial_state = row['parsed_initial_state']
        goal_state = row['parsed_solution_state']
        puzzle_type = row['puzzle_type']
        num_wildcards = row['num_wildcards']
        allowed_moves = puzzle_info_df[puzzle_info_df['puzzle_type'] == puzzle_type]['allowed_moves'].iloc[0]

        solution_moves = None

        # Attempt to find a solution
        if index < limit_index:
            solution_moves = improved_a_star_search_with_wildcards(initial_state, goal_state, allowed_moves,
                                                                   num_wildcards)

        # If no solution found, use the sample submission's result
        if solution_moves is None:
            solution_moves = target_submission_df[target_submission_df['id'] == puzzle_id]['moves'].iloc[0].split('.')
            solution_moves = [(move.replace('-', ''), move.startswith('-')) for move in solution_moves]

        formatted_solution = format_solution_for_submission(puzzle_id, solution_moves)
        solutions.append(formatted_solution)

    return pd.DataFrame(solutions)

########################################
def improve_solution_kstep(original_solution_df: object, puzzle_info_df: object, puzzles: object, puzzle_index: object,
                           solution_block_size: object = 10) -> object:
    """
    Improve an existing solution using K-step Iterative Search

    :param original_solution_df: DataFrame containing the original set of solutions to be improved upon.
    :param puzzle_info_df: DataFrame containing allowed moves for each puzzle type.
    :param puzzles: The collection of puzzles
    :param puzzle_index: Which puzzle/solution to improve
    :param solution_block_size: The length of the solution's sub-path to improve upon

    :return: DataFrame with the solutions formatted for submission.
    """

    orig_solution = original_solution_df['moves'][puzzle_index].split('.')
    initial_state = puzzles['parsed_initial_state'][puzzle_index]
    goal_state = initial_state.copy()
    puzzle_type = puzzles['puzzle_type'][puzzle_index]
    puzzle_id = puzzles['id'][puzzle_index]
    allowed_moves = puzzle_info_df[puzzle_info_df['puzzle_type'] == puzzle_type]['allowed_moves'].iloc[0]
    num_wildcards = puzzles['num_wildcards'][puzzle_index]
    final_block = False
    optimized_path = []

    for move_number in tqdm(range(0, len(orig_solution), solution_block_size),
                            total = int(len(orig_solution) / solution_block_size),
                            desc = f'Optimizing puzzle #{puzzle_index}'):

        initial_state = goal_state.copy()

        if move_number + solution_block_size > len(orig_solution):
            move_block = orig_solution[move_number:]
            goal_state = puzzles['parsed_solution_state'][puzzle_index]
            final_block = True
        else:
            move_block = orig_solution[move_number:move_number + solution_block_size]
            for move in move_block:
                inverse = False
                if move.startswith('-'):
                    inverse = True
                    move = move[1:]
                goal_state = apply_move(goal_state, allowed_moves[move], inverse)

        optimized_block = None

        # Attempt to find a solution
        if final_block:
            # add in wild cards for last block of the path
            optimized_block = improved_a_star_search_with_wildcards(initial_state, goal_state, allowed_moves,
                                                                   num_wildcards, max_depth = len(move_block)- 1)
        else:
            optimized_block = a_star_search_with_timeout(initial_state, goal_state, allowed_moves,
                                                        max_depth = solution_block_size - 1)

        # If no shorter solution was found, use the original path
        if optimized_block is None:
            optimized_block = move_block
            optimized_path.extend([(move.replace('-', ''), move.startswith('-')) for move in optimized_block])
        else:
            optimized_path.extend(optimized_block)

    formatted_solution = format_solution_for_submission(puzzle_id, optimized_path)
    return formatted_solution

########################################
# sample_submission_df = pd.read_csv(sample_submission_path)
orig_submission_path = '../ensemble/ensemble_240101.csv'
orig_submission_df = pd.read_csv(orig_submission_path)

# Need to add a submission output

kstep_results_path = 'kstep_cube_5x5x5_256.csv' 
new_df = pd.DataFrame(columns = ['index', 'orig_sol', 'orig_sol_len', 'new_sol', 'new_sol_len', 'sol_len_improvement'])

for puzzle_index in [256]: # range(245, 247):
    new_solution = improve_solution_kstep(orig_submission_df, puzzle_info_df, puzzles_df,
                                          puzzle_index = puzzle_index, solution_block_size = 10)

    orig_sol_len = len(orig_submission_df['moves'][puzzle_index].split('.'))
    new_sol_len = len(new_solution['moves'].split('.'))
    sol_len_improvement = orig_sol_len - new_sol_len

    new_df.loc[len(new_df)] = {'index': puzzle_index,
                               'orig_sol': orig_submission_df['moves'][puzzle_index],
                               'orig_sol_len': orig_sol_len,
                               'new_sol': new_solution['moves'],
                               'new_sol_len': new_sol_len,
                               'sol_len_improvement': sol_len_improvement}

    print('\n', new_df.loc[len(new_df) - 1], '\n')

new_df.to_csv(kstep_results_path, index = False)
