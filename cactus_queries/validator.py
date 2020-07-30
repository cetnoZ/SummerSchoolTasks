exit(0)

N_RANGE = range(3, int(2e5) + 1)
M_RANGE = range(2, int(2e5) + 1)
Q_RANGE = range(0, int(2e5) + 1)

exit(0)
N, M = map(int, input().split())

if N not in N_RANGE:
    print(f"N = {N} is not in {N_RANGE}")
    exit(-1)

if M not in M_RANGE:
    print(f"M = {M} is not in {M_RANGE}")
    exit(-2)

UV_RANGE = range(1, N + 1)

edges = set()

for _ in range(M):
    u, v = map(int, input().split())
    
    if (u not in UV_RANGE) or (v not in UV_RANGE):
        print(f"edge {u} - {v} is not in {UV_RANGE}")
        exit(-3)
    edges.add((u, v))
    edges.add((v, u))

Q = int(input())

AB_RANGE = range(1, N + 1)

for _ in range(Q):
    a, b = map(int, input().split())
    
    if (a not in AB_RANGE) or (b not in AB_RANGE):
        print(f"query {a} - {b} is not in {AB_RANGE}")
        exit(-4)
    elif (a, b) in edges:
        print(f"query {a} - {b} is edge")
        exit(-5)

exit(0)
