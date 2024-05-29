import numpy as np # linear algebra
import pandas as pd # data processing, CSV file I/O (e.g. pd.read_csv)
import matplotlib.pyplot as plt
import time
import heapq
import random
random.seed(1)

import os

# Load Data
INPUT_DIR = '.'
puzzles_df = pd.read_csv(f'{INPUT_DIR}/puzzles.csv')
puzzle_info_df = pd.read_csv(f'{INPUT_DIR}/puzzle_info.csv')
ss_df = pd.read_csv(f'{INPUT_DIR}/sample_submission.csv')
df_dict = {
    "Puzzles": puzzles_df,
    "Puzzle Info": puzzle_info_df,
    "Sample submission": ss_df,
}

# Split up the initial state
puzzles_df['initial_state_parsed'] = puzzles_df['initial_state'].apply(lambda x: x.split(";"))
# Split up the solution state
puzzles_df['solution_state_parsed'] = puzzles_df['solution_state'].apply(lambda x: x.split(";"))
# Get total number of 'faces' to be solved
puzzles_df['total_components'] = puzzles_df['solution_state_parsed'].apply(lambda x: len(x))
# Get a list of 'colours'
puzzles_df['unique_components'] = puzzles_df['solution_state_parsed'].apply(lambda x: np.unique(x))
# How many 'colours' are there?
puzzles_df['total_unique_components'] = puzzles_df['unique_components'].apply(lambda x: len(x))

# Convert allowed moves from string to dict
import json
def get_allowed_moves(dict_string):
    moves_dict = json.loads(dict_string.replace("'", '"'))
    # add inverse moves
    for move in list(moves_dict.keys()):
        inverse_move = list(np.argsort(moves_dict[move]))
        if inverse_move not in moves_dict.values():
            moves_dict['-' + move] = inverse_move
    return moves_dict
puzzle_info_df['allowed_moves_dict'] = puzzle_info_df['allowed_moves'].apply(lambda x: get_allowed_moves(x))
# Get total number of allowed moves
puzzle_info_df['total_allowed_moves'] = puzzle_info_df['allowed_moves_dict'].apply(lambda x: len(x.keys()))

# Split up sample submission into list
ss_df['moves_parsed'] = ss_df['moves'].apply(lambda x: x.split('.'))
# Count moves in sample submission solutions
ss_df['total_moves'] = ss_df['moves_parsed'].apply(lambda x: len(x))

def apply_move(state, move):
    # The move vector tells us what index to sample the next state from
    return [state[i] for i in move]


def apply_move_sequence(state, move_sequence, move_dict):
    # Apply a series of moves when given them as a string
    new_state = state
    for move_str in move_sequence.split('.'):
        move = move_dict.get(move_str)
        new_state = apply_move(new_state, move)
    return new_state

def evaluate_difference(current_state, final_state, phase):
    if phase == 'edges':
        return evaluate_edges(current_state, final_state)
    elif phase == 'corners':
        return evaluate_corners(current_state, final_state)
    else:
        return max(0, sum(s != g for s, g in zip(current_state, final_state)))

def evaluate_score(current_state, final_state, phase):
    # Reward having the final position match, and also reward having 2 of the same state adjacent to each other
    # This has to be fast since it's called so often
    if phase == 'edges':
        return evaluate_edges(current_state, final_state)
    elif phase == 'corners':
        return evaluate_corners(current_state, final_state)
    else:
        return max(0, sum(s != g for s, g in zip(current_state, final_state)))

    # return max(0, sum(s != g for s, g in zip(current_state, final_state))) # + sum(
        # s != g for s, g in zip(current_state[1:], current_state[:-1])) + 0.5 * sum(
        # s != g for s, g in zip(current_state[2:], current_state[:-2])))

def evaluate_edges(current_state, final_state):
    edges_and_middles = [1, 3, 4, 5, 7, 10, 12, 13, 14, 16, 19, 21, 22, 23, 25, 28, 30, 31, 32, 34, 37, 39, 40, 41, 43, 46, 48, 49, 50, 52]
    return max(0, sum(s != g for i, (s, g) in enumerate(zip(current_state, final_state)) if i in edges_and_middles))

def evaluate_corners(current_state, final_state):
    edges_and_middles = [1, 3, 4, 5, 7, 10, 12, 13, 14, 16, 19, 21, 22, 23, 25, 28, 30, 31, 32, 34, 37, 39, 40, 41, 43, 46, 48, 49, 50, 52]
    return max(0, sum(s != g for i, (s, g) in enumerate(zip(current_state, final_state)) if i not in edges_and_middles))

def idastar(move_dict, initial_state, final_state, params):
    # Priority queue to store nodes with their f-values (g + h)
    puzzle_start_time = time.time()
    iteration_counter = 0
    current_starting_state = initial_state
    current_path = []
    closed_set = set()
    open_set = []
    params['phase'] = 'all'
    temp_move_dict = move_dict.copy()

    while time.time() - puzzle_start_time < params['max_overall_time'] and iteration_counter < params['max_overall_iterations']:

        iteration_counter += 1

        print('Difference: ', evaluate_difference(final_state, current_starting_state, params['phase']))
        # print(np.array(current_starting_state).reshape(6, 3, 3))
        # Set the cutoff to be a fraction of the intial start cost
        if params['phase'] == 'corners' and 'r1' in move_dict.keys():
            for k in ['r1', '-r1', 'd1', '-d1', 'f1', '-f1']:
                del move_dict[k]
        else:
            move_dict = temp_move_dict.copy()

        new_state, new_path, node_counter = depth_limited_search(move_dict, current_starting_state, final_state,
                                                                 open_set, closed_set, params)

        if new_state is None:
            print("Depth limited search failed. Using sample submission results instead.")
            return None, iteration_counter

        current_path += new_path
        current_difference = evaluate_difference(new_state, final_state, params['phase'])
        current_score = evaluate_score(new_state, final_state, params['phase'])
        current_starting_state = new_state
        # Remove every 2nd item from the closed set, so that
        # closed_set = set(random.sample(list(closed_set), int(len(closed_set) * params['set_downsampling'])))
        print(
            f"Iteration #{iteration_counter} completed - New Moves: {len(new_path)}, Total Moves: {len(current_path)}, Difference: {current_difference}, Score: {current_score}, Nodes: {node_counter}")

        if current_difference <= params['wildcards']:
            if params['phase'] == 'corners':
                # print(np.array(current_starting_state).reshape(6, 3, 3))
                # print('***** Phase 1 complete *****')
                params['phase'] = 'all'
            else:
                # We've achieved our goal. Return the move path.
                # print(np.array(current_starting_state).reshape(6, 3, 3))
                # print('***** Phase 2 complete *****')
                return current_path, iteration_counter
    print(
        f"Max search time/iterations expired: {time.time() - puzzle_start_time} seconds, {iteration_counter} iterations.")
    return None, iteration_counter

def depth_limited_search(move_dict, initial_state, final_state, open_set, closed_set, params):
    # Priority queue to store nodes with their f-values (g + h)
    start_time = time.time()
    node_counter = len(open_set) + 1

    heapq.heappush(open_set, (0, initial_state, []))  # (priority, state, path)
    # Set to keep track of visited nodes

    while open_set:
        # Get the node with the lowest f-value
        current_f, current_state, current_path = heapq.heappop(open_set)

        # Check for timeout
        # if time.time() - start_time > params['max_iteration_time']:
            #             print("Iteration Timed Out.")
            # return current_state, current_path, node_counter

        if node_counter > params['max_iteration_nodes']:
            print("Iteration Node Limit Reached.")
            # smallest = heapq.nsmallest(int(len(open_set) * .5), open_set)
            # open_set.clear()
            # open_set.extend(smallest)
            # heapq.heapify(open_set)
            return current_state, current_path, node_counter

        if evaluate_difference(current_state, final_state, params['phase']) <= params['wildcards']:
            # We've achieved our goal. Return the move path.
            return current_state, current_path, node_counter

        closed_set.add(tuple(current_state))

        for move_str, move in move_dict.items():
            new_state = apply_move(current_state, move)
            if tuple(new_state) not in closed_set:
                heapq.heappush(open_set, (
                len(current_path) + 1 + evaluate_score(new_state, final_state, params['phase']), new_state, current_path + [move_str]))
                node_counter += 1
    # If no solutions are found:
    print("Open set completed. No solutions.")
    return None, None, node_counter

# Puzzle solver automated
def solve_puzzles(puzzles_df, puzzle_info_df):
    solution_list = []
    for puzzle_number in np.arange(len(puzzles_df)):
        solution_dict = {'id': puzzle_number}
        ss_moves = len(ss_df[ss_df['id'] == puzzle_number]['moves_parsed'].iloc[0])
        # ss_moves > 1500:
        # if (puzzles_df.puzzle_type.iloc[puzzle_number] not in ['cube_2/2/2'] and
        #    not puzzles_df.puzzle_type.iloc[puzzle_number].startswith('wreath')):
        if puzzle_number < 284 or puzzle_number > 328:  # pick puzzles of interest
            solution_path = ss_df[ss_df['id'] == puzzle_number]['moves_parsed'].iloc[0]
            solution_moves = ".".join(solution_path)
            self_solved = False
            solution_dict = {
                'id': puzzle_number,
                'moves': solution_moves,
                'move_total': len(solution_path),
                'ss_moves': ss_moves,
                'self_solved': False,
                'time_taken': 0,
                'iteration_counter': 0
            }
            solution_list.append(solution_dict)

            print(
                f"Puzzle #{puzzle_number} skipped since expected moves is {ss_moves} and we think we'll be too slow. Using sample submission solution instead.")
            continue

        puzzle_start_time = time.time()
        move_sequence = ss_df.moves_parsed.iloc[puzzle_number]
        initial_state = puzzles_df.initial_state_parsed.iloc[puzzle_number]
        final_state = puzzles_df.solution_state_parsed.iloc[puzzle_number]
        puzzle_type = puzzles_df.puzzle_type.iloc[puzzle_number]
        wildcards = puzzles_df.num_wildcards.iloc[puzzle_number]
        move_dict = puzzle_info_df.loc[puzzle_info_df['puzzle_type'] == puzzle_type]['allowed_moves_dict'].iloc[0]
        params = {
            'wildcards': wildcards,
            'max_iteration_time': 30,
            'max_iteration_nodes': 12500000,
            'max_overall_time': 300,
            'max_overall_iterations': 50,
            'max_moves': ss_moves,
            'set_downsampling': 0.80
        }
        solution_path, iteration_counter = idastar(move_dict, initial_state, final_state, params)
        if solution_path is not None:
            solution_moves = ".".join(solution_path)
            self_solved = True
            print(
                f"Puzzle #{puzzle_number} solved in {len(solution_path)} moves. {(time.time() - puzzle_start_time):.2f} seconds, {iteration_counter} iterations. {solution_path}")
        else:
            # If no solution found, use the sample submission's result
            solution_path = ss_df[ss_df['id'] == puzzle_number]['moves_parsed'].iloc[0]
            solution_moves = ".".join(solution_path)
            self_solved = False
            print(
                f"Puzzle #{puzzle_number} failed after {(time.time() - puzzle_start_time):.2f} seconds, {iteration_counter} iterations. Using sample submission solution instead.")
        solution_dict = {
            'id': puzzle_number,
            'moves': solution_moves,
            'move_total': len(solution_path),
            'ss_moves': ss_moves,
            'self_solved': self_solved,
            'time_taken': time.time() - puzzle_start_time,
            'iteration_counter': iteration_counter,
        }
        solution_list.append(solution_dict)
    solution_df = pd.DataFrame(solution_list)
    return solution_df

solution_df = solve_puzzles(puzzles_df, puzzle_info_df)
solution_df.head(400)

print(f"Total moves: {solution_df['move_total'].sum()}")
print(f"Total sample solution moves: {solution_df['ss_moves'].sum()}")
print(f"Improvement compared to sample solutions: {100-100*solution_df['move_total'].sum()/solution_df['ss_moves'].sum():.2f}%")
print(f"Total solved: {solution_df['self_solved'].sum()}")
print(f"Total time taken: {solution_df['time_taken'].sum()/60:.2f} minutes")

# Compile into csv
final_solution_df = solution_df[['id', 'moves']]
final_solution_df.to_csv("submission_wreath12x12.csv", header=['id', 'moves'], index=False)
print(final_solution_df)