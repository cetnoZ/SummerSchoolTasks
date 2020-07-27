# Path rotation

You are given a binary tree consisting of n vertices with a root in vertex 1. Each vertex has its own weight factor w_v. 

Path rotation is deefined as operation on path between two different operations. By executing rotation between u and v, weights along the path are swapped in cycle manner, i. e. w_path_2 = w_path_1, w_path_3 = w_path_2 ...

Your task is to answer queries described by two different vertices. Answer to the query u_i, v_i is minimum sum of weights you can achieve by executing at most one path rotation that do not start or end on any vertex along the path between vertices v_i and u_i.


### Input

The first line of the input contains one integer n (1 <= n <= 5 * 10 ^ 3) -- the number of vertices in the tree.

The next n line describes each vertex. i-th line contains three integers l_i, r_i and w_i (0 <= l_i, r_i <= n, 0 <= w_i <= 10 ^ 9) -- left and right childs and weight of i-th vertex. If vertex has no left of right child value of l_i or r_i will be equal to 0 accordingly.

The following line contains number of queries -- q (0 <= q <= 5 * 10 ^ 3).

Next q lines describes queries in format u_i, v_i (0 <= u_i, v_i <= n, u_i != v_i).

Its guaranteed that input is correct, and given data represents valid tree with root in vertex 1.


### Output

For each query output minimum sum of weights you can achieve by executing at most one path rotation 
along path that nor start neither end on given path.
