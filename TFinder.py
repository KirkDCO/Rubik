from itertools import permutations

def inverse(seq):
    return [seq.index(v + 1) + 1 for v in range(len(seq))]


A = ['A', 'B', 'C', 'D']
B = ['A', 'B', 'D', 'C']
C = ['A', 'C', 'B', 'D']
D = ['A', 'C', 'D', 'B']
E = ['A', 'D', 'B', 'C']
F = ['A', 'D', 'C', 'B']
states = [A, B, C, D, E, F]
state_names = ['A', 'B', 'C', 'D', 'E', 'F']

i = [1, 2, 3, 4]
a = [3, 1, 4, 2]

comms = [i, a, inverse(a), [4, 3, 2, 1]]
comm_names = ['i', 'a', 'a_inv', 'aa']

for i, s in enumerate(states):
    if s not in [B, C, F]:
        continue
    start = s
    end = F
    for j, c in enumerate(comms):
        for p in list(permutations([1, 2, 3, 4])):
            for p2 in list(permutations([1,2,3,4])):
                int_1 = [start[v - 1] for v in p]
                int_1b = [int_1[v - 1] for v in p2]
                int_2 = [int_1b[v - 1] for v in c]
                int_3 = [int_2[v - 1] for v in inverse(p2)]
                int_3b = [int_3[v - 1] for v in inverse(p)]
                final = [int_3b[v - 1] for v in inverse(c)]

                if final == end:
                    print(f'State {state_names[i]}, Comm {comm_names[j]} Solution: {p}')
                    print(start)
                    print(int_1)
                    print(int_1b)
                    print(int_2)
                    print(int_3)
                    print(int_3b)
                    print(final)
                    print()