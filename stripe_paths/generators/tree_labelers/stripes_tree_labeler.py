import random
import itertools
import math
from collections import deque
from .tree_labeler import *
from .random_tree_labeler import *


class StripesTreeLabeler(TreeLabeler):
    def __init__(self, stripes_count, k_type):
        TreeLabeler.__init__(self)
        self._random_labeler = RandomTreeLabeler(0.5)
        self._k_type = k_type
        self._stripes_count = stripes_count

    def label_tree(self, root):
        self._random_labeler.label_tree(root)

        depths = self.calc_depths(root)
        depths_keys = list(depths.keys())
        weights = list(map(len, depths.values()))
        accumulated_weights = list(itertools.accumulate(weights))
        depth = max(depths_keys)

        for _ in range(self._stripes_count):
            k = self.generate_k(self._k_type, depth)
            vertex = self.pick_vertex(k, depths, depths_keys, accumulated_weights)
            up_cnt = self.pick_up_cnt(k, vertex)
            self.label_up(vertex, k, up_cnt)

        return root

    def generate_k(self, k_type, depth):
        if k_type == 'average':
            median_k = round(math.sqrt(depth))
        elif k_type == 'big':
            median_k = round(depth / random.randint(2, 10))
        elif k_type == 'small':
            median_k = random.randint(1, 10)
        elif k_type == 'random':
            return self.generate_k(random.choice(['average', 'big', 'small']), depth)

        median_k = min(depth // 2, max(1, median_k))
        delta_k = max(2, median_k // 10)

        min_k = max(1, median_k - delta_k)
        max_k = max(min_k, min(depth // 2, median_k + delta_k))

        k = random.randint(min_k, max_k)

        return k

    def pick_vertex(self, k, depths, depths_keys, accumulated_weights):
        depth = random.choices(depths_keys[2 * k - 1:], cum_weights=accumulated_weights[2 * k - 1:])[0]
        vertex = random.choice(depths[depth])
        return vertex

    def pick_up_cnt(self, k, vertex):
        min_up_cnt = 1
        max_up_cnt = vertex.depth // (2 * k)
        return random.randint(min_up_cnt, max_up_cnt)
