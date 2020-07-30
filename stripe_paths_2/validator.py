N_RANGE = range(1, int(3e4) + 1)

N = int(input())

if N not in N_RANGE:
    print(f"N = {N} is not in {N_RANGE}")
    exit(-1)

UV_RANGE = range(0, N + 1)

for _ in range(1, N + 1):
    u, v = map(int, input().split())
    if (u not in UV_RANGE) or (v not in UV_RANGE):
        print(f"childs {u} {v} is not in {UV_RANGE}")
        exit(-2)

Q = int(input())
UV_RANGE = range(1, N + 1)
K_RANGE = range(1, (N - 1) // 2 + 1)

for _ in range(1, Q + 1):
    query = list(map(int, input().split()))
    if query[0] == 1:
        t, u, v = query
        if (u not in UV_RANGE) or (v not in UV_RANGE):
            print(f"childs {u} {v} is not in {UV_RANGE}")
            exit(-2)
    elif query[0] == 2:
        t, u, v, k = query
        if (u not in UV_RANGE) or (v not in UV_RANGE):
            print(f"childs {u} {v} is not in {UV_RANGE}")
            exit(-2)
    else:
        print(f"unknown query type {query[0]}")
        exit(-1)

exit(0)
