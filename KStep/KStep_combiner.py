import os
import pandas as pd

with open('../sample_submission.csv', 'r') as sample_sub:
    sub_df = pd.read_csv(sample_sub)

kstep_files = os.listdir()
count = 0
total_improvement = 0
for fn in kstep_files:
    if fn.startswith('kstep') and fn.endswith('.csv'):
        with open(fn, 'r') as fin:
            kstep_df = pd.read_csv(fn)

        for idx, row in kstep_df.iterrows():
            index = row.loc['index']
            kstep_sol = row.loc['new_sol']
            improve = row.loc['sol_len_improvement']

            sub_df.iloc[index, 1] = kstep_sol

            print(f'Index #{index} improvement: {improve}')
            count += 1
            total_improvement += improve

sub_df.to_csv('kstep_ensemble_240111.csv')
print(f'{count} puzzles  {total_improvement} improvement')
