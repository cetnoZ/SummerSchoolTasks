# Stripe paths

You are given a binary tree consisting of n vertices with a root in vertex 1. Each vertex has its own weight factor w_v. 

Rotation of two adjacent vertices u and v in the tree is defined as following action. Lets suppose height of vertex v is greater than vertex u, then after applying rotation vertex u will be located higher than v, and relative order of u and v subtrees will remain the same.

      p                  p
      |                  |
      v                  u
     / \                / \
    A   u      <=>     v   C
       / \            / \
      B   C          A   B


Your task is to give a sequence of rotations to minimize cost of the tree. For every tree cost can be calculated by using the following formula:
       
Cost =   E    w[v] * h[v]
      v in V

As seen from the formula cost is sum of height and weight of every vertex in the tree.


### Input

The first line of the input contains one integer n (1 <= n <= 10 * 10 ^ 3) -- the number of vertices in the tree.

The next n line describes each vertex. i-th line contains three integers l_i, r_i and w_i (1 <= l_i, r_i <= n, 0 <= w_i <= 10 ^ 9) -- left and right childs and weight of i-th vertex.

Its guaranteed that input is correct, and given data represents valid tree with root in vertex 1.


### Output

First line of the output contains should contain one integer t (0 <= t <= 2 * 10 ^ 5) number of rotations.

Each next t lines should contain two integers v_i, u_i (1 <= v_i, u_i <= n), which represent rotations.

If there are multiple ways to achieve minimum cost, output any of them.
