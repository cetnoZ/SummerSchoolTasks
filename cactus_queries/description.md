# Cactus queries


### Input

The first line contains two integer n and m (2 <= n, m <= 2 * 10 ^ 5) -- the number of the vertices and number of edges in the grapg.

The following m lines descibe one edge. Each edge consists of two integers v_i and u_i (1 <= u_i, v_i <= n, v_i != u_i).

Next line contains one integer q (0 <= q <= 2 * 10 ^ 5) -- the number of queries.

Next q lines describes queries. i-th query contain two integers a_i and b_i (1 <= a_i, b_i <= n) -- vertices to connect.

Input is guaranteed to be correct and graph it represents is connected, and every vertex belogns to at most one cycle. 

### Output

For each query output "Yes" if connecting vertices a_i and b_i will not break graph property described above, and "No" otherwise.
