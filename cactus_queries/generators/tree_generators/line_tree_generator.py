import random
from .tree_generator import *


class LineTreeGenerator(TreeGenerator):
    def __init__(self, depth_coeff=1):
        TreeGenerator.__init__(self)
        self._depth_coeff = depth_coeff

    def generate(self, vertex_count):
        root = self.create_node()
        vertex = root

        for _ in range(vertex_count - 1):
            vertex, parent = self.create_node(), vertex
            parent.add_child(vertex)

        return root
