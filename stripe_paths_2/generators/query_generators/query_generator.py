from tree import *
from math import *

class QueryGenerator:
    def __init__(self):
        self._K = 20

    def generate_queries(self, root, count):
        self._vertex_count = count_vertices(root)

        self._parents = [[0 for _ in range(self._vertex_count + 1)] for _ in range(self._K + 1)]
        self._ltime = [0] * (self._vertex_count + 1)
        self._rtime = [0] * (self._vertex_count + 1)
        self._time = 0

        self._vertices = {node.index: node for node in tree_nodes(root)}

        self.precalc_lca(root)
        return self.generate_all_queries(root, count)

    def generate_all_queries(self, root, count):
        pass

    def inside(self, higher, lower):
        return self._ltime[higher] <= self._ltime[lower] and self._rtime[lower] <= self._rtime[higher]

    def precalc_lca(self, node, parent=None):
        if node is None:
            return

        self._time += 1
        self._ltime[node.index] = self._time

        self._parents[0][node.index] = parent.index if parent is not None else 0
        for j in range(1, self._K):
            self._parents[j][node.index] = self._parents[j - 1][self._parents[j - 1][node.index]]

        self.precalc_lca(node.left_node, node)
        self.precalc_lca(node.right_node, node)

        self._rtime[node.index] = self._time

    def lca(self, u, v):
        if self.inside(u, v):
            return u
        if self.inside(v, u):
            return v
        
        for j in range(self._K - 1, -1, -1):
            upper = self._parents[j][v]
            if upper == 0 or self.inside(upper, u):
                continue
            v = upper
        return self._parents[0][v]

    def find_divisors(self, k):
        for i in range(1, round(sqrt(k)) + 1):
            if k % i == 0:
                if (k // i) * (k // i) == k:
                    yield i
                else:
                    yield i
                    yield k // i
