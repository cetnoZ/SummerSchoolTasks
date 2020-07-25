import random
from tree import *
from .tree_weighter import *


class RandomTreeWeighter(TreeWeighter):
    def __init__(self, max_weight, percent=0.1):
        TreeWeighter.__init__(self)
        self._max_weight = max_weight
        self._percent = percent

    def set_weights(self, root):
        for vertex in tree_nodes(root):
            vertex.weight = self.pick_weight()
        return root

    def pick_weight(self):
        min_weight = max(0, round(self._max_weight * (1 - self._percent)))
        max_weight = self._max_weight
        return random.randint(min_weight, max_weight)
