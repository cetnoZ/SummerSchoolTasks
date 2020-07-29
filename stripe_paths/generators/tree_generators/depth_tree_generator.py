import random
from .tree_generator import *


class DepthTreeGenerator(TreeGenerator):
    def __init__(self, depth_coeff=1):
        TreeGenerator.__init__(self)
        self._depth_coeff = depth_coeff

    def generate(self, vertex_count):
        root = self.create_node()
        vertices = [root]
        accumulated_weights = [1]

        for _ in range(vertex_count - 1):
            node = self.create_node()
            parent = random.choices(vertices, cum_weights=accumulated_weights)[0]

            parent.add_child(node)
            vertices.append(node)
            accumulated_weights.append(accumulated_weights[-1] + self._depth_coeff * node.depth)
        return root
