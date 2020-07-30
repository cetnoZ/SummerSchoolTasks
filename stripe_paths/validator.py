N_RANGE = range(1, int(1e5) + 1)

N = int(input())

if N not in N_RANGE:
    print(f"N = {N} is not in {N_RANGE}")
    exit(-1)

UV_RANGE = range(0, N + 1)
C_RANGE = range(0, 2)

edges = set()

for _ in range(1, N):
    u, v, c = map(int, input().split())
    if (u not in UV_RANGE) or (v not in UV_RANGE):
        print(f"childs {u} {v} is not in {UV_RANGE}")
        exit(-2)
    elif c not in C_RANGE:
        print(f"c = {c} is not in {C_RANGE}")
        exit(-2)
    elif (u, v) in edges:
        print(f"duplicate edge {u} - {v}")
        exit(-3)
    edges.add((u, v))
    edges.add((v, u))

exit(0)
