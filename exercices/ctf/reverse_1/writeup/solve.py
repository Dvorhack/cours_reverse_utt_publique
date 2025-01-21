from z3 import BitVec, Solver, Distinct
import re

password = [BitVec(f"x_{i}", 8) for i in range(6*6)]

s = Solver()

for i in range(6*6):
    s.add(password[i] > 0)
    s.add(password[i] < 7)

buffer_in = [
     6, None, None,  5, None,  2,
    None,  2, None,  1, None, None,
    None, None,  2, None, None, None,
    None,  5, None,  2,  1,  6,
    None,  3, None,  6,  5,  4,
     5,  4,  6, None,  2,  1,
]
for i, e in enumerate(buffer_in):
    if e is not None:
        s.add(password[i] == e)

# check number in each column is unique
for column in range(6):
    x = []
    for raw in range(6):
        x.append(password[6*raw + column])
    print(x)
    s.add(Distinct(x))

# check number in each raw is unique
for raw in range(6):
    x = []
    for column in range(6):
        x.append(password[6*raw + column])
    print(x)
    s.add(Distinct(x))

s.check()
m = s.model()
m = re.findall(b'x_(\d+) = (\d+)', str(m).encode())
m = sorted(m, key=lambda x: int(x[0].decode()))
print(m)
password = [int(x[1].decode()) for x in m]

# Print as a sudoku grid
for raw in range(6):
    for col in range(6):
        print(f"{password[raw*6+col]:3}", end=" ")
    print()

    
print(''.join(str(x) for x in password))