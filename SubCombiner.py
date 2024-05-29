Cube2x2x2_Wreath = open('submission_IDAstar_2x2x2_wreath_12x12.csv', 'r')
NxNxN = open('submission_IDAstar_NxNxN.csv')
wreath12x12 = open('submission_wreath12x12.csv')

h = Cube2x2x2_Wreath.readline()
_ = NxNxN.readline()
_ = wreath12x12.readline()

submission = open('submission_NxNxN_wreath12x12', 'w')
submission.write(h)

move_count = 0

for puzzle_number, (file1_line, file2_line, file3_line) in enumerate(zip(Cube2x2x2_Wreath, NxNxN, wreath12x12)):
    file1_tokens = file1_line.strip().split(',')
    file2_tokens = file2_line.strip().split(',')
    file3_tokens = file3_line.strip().split(',')

    file1_moves = len(file1_tokens[1].split('.'))
    file2_moves = len(file2_tokens[1].split('.'))
    file3_moves = len(file3_tokens[1].split('.'))

    if file1_moves == min(file1_moves, file2_moves, file3_moves):
        submission.write(','.join([file1_tokens[0], file1_tokens[1]]) + '\n')
        move_count += file1_moves
    elif file2_moves == min(file1_moves, file2_moves, file3_moves):
        submission.write(','.join([file2_tokens[0], file2_tokens[1]]) + '\n')
        move_count += file2_moves
    elif file3_moves == min(file1_moves, file2_moves, file3_moves):
        submission.write(','.join([file3_tokens[0], file3_tokens[1]]) + '\n')
        move_count += file3_moves

submission.close()
wreath12x12.close()
NxNxN.close()
Cube2x2x2_Wreath.close()

print(f'Moves: {move_count}')