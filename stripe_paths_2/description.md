# Stripe paths 2

You are gven a binary tree consisting of n vertices. Root of the tree is located in vertex with index 1.

"Stripe" pattern is defined as some positive count of identical segments in a sequence, where each segment contains k red colors following k green colors.

For example, first sequence of colors is corrent "stripe" sequence, while second is not:
    red red green green red red green greed
    green red green green

"Stripe" path is path between two vertices in the tree vertices which edges makes sequence of colors represenets "stripe" pattern.

Initially all edges of the tree have green color. Your task is to execute two kinds of queries:
    * 1 u_i v_i -- check if path between given vertices form "stripe" sequence.
    * 2 u_i v_i k_i -- change colors of path from u_i to v_i to "stripe" sequence with length of equal color each group segments is equal to k_i.


### Input

The first line of the input contains one integer n (1 <= n <= 3 * 10^4) -- the number of vertices in the tree.

The next n lines describes vertex in the following format. i-th line contains two integers l_i, r_i (0 <= l_i, r_i <= n) -- left and right childs of vertex i. If left or right child doesnt exists then child vertex will be 0 and color will be 0. 0 color stands for green, while 1 represents red.

Next line contains one integer q (0 <= q <= 3 * 10 ^ 4)-- number of queries.

Following q lines describes queries in one of two formats:
    * 1 u_i v_i (1 <= u_i, v_i <= n, u_i != v_i) -- query of first type where u_i and v_i describes ends of path to check.
    * 2 u_i v_i k_i (1 <= u_i, v_i <= n, u_i != v_i, 1 <= k_i <= n) -- query of second type where u_i, v_i -- ends of path on which you need to change colors, k_i -- length of identical color groups.

Its guaranteed that the given set of edges represents form a tree. And queries of second type contains k_i equal to such integer to length of path is divisible by k_i.


### Output

For each query of type 1 print "Yes" if path described by vertices represents "stripe" path.
