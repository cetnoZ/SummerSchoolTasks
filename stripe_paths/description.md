# Stripe paths

You are gven a tree consisting of n vertices. Root of the tree is located in vertex with index 1.

"Stripe" pattern is defined as some positive count of identical segments in a sequence, where each segment contains k red colors following k green colors.

For example, first sequence of colors is corrent "stripe" sequence, while second is not:
    red red green green red red green greed
    green red green green

Your task is to count all paths in tree, which starts in some vertex in the tree and goes only towards one of leaf vertices and sequence of colors represents "stripe" pattern.


### Input

The first line of the input contains one integer n (1 <= n <= 10^5) -- the number of vertices in the tree.

The next n - 1 lines describes edges. The i-th edge is represented as three integers x_i, y_i and c_i (1 <= x_i, y_i <= n, 0 <= c_i <= 1), where x_i and y_i are vertices i-th edge connects, c_i is the color of the edge.

Its guaranteed that the given set of edges represents form a tree.


### Output

Count of described paths in the given tree.


### Note

Notice however that described path might not end exactly in the leaf. Also "stripe" sequence should be only counted considering edges from upper vertex down to the end of the path.
