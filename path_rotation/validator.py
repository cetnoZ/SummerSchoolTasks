N_RANGE = range(1, int(2e5) + 1)
Q_RANGE = range(0, int(2e5) + 1)

N = int(input())

if N not in N_RANGE:
    print(f"N = {N} is not in {N_RANGE}")
    exit(-1)

UV_RANGE = range(0, N + 1)
W_RANGE = range(0, int(1e9) + 1)

parent_counts = {}

for i in range(1, N + 1):
    u, v, w = map(int, input().split())
    if (u not in UV_RANGE) or (v not in UV_RANGE):
        print(f"childs {u} {v} is not in {UV_RANGE}")
        exit(-2)
    elif w not in W_RANGE:
        print(f"w = {w} is not in {W_RANGE}")
        exit(-2)

Q = int(input())

if Q not in Q_RANGE:
    print(f"Q = {Q} is not in {Q_RANGE}")
    exit(-3)

AB_RANGE = range(1, N + 1)

for _ in range(Q):
    a, b = map(int, input().split())
    
    if (a not in AB_RANGE) or (b not in AB_RANGE):
        print(f"query {a} - {b} is not in {AB_RANGE}")
        exit(-4)
    elif a == b:
        print(f"query {a} - {b} is loop")
        exit(-5)

exit(0)
