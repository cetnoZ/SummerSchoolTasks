N_RANGE = range(1, int(500) + 1)
W_RANGE = range(0, int(1e9) + 1)

N = int(input())

if N not in N_RANGE:
    print(f"N = {N} is not in {N_RANGE}")
    exit(-1)

UV_RANGE = range(0, N + 1)
C_RANGE = range(0, 2)

for _ in range(1, N):
    u, v, w = map(int, input().split())
    if (u not in UV_RANGE) or (v not in UV_RANGE):
        print(f"childs {u} {v} is not in {UV_RANGE}")
        exit(-2)
    elif w not in W_RANGE:
        print(f"w = {w} is not in {W_RANGE}")
        exit(-2)

exit(0)
