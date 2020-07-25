import random
from .tree_generator import *


class DepthTreeGenerator(TreeGenerator):
    def __init__(self, depth_coeff=1):
        TreeGenerator.__init__(self)
        self._depth_coeff = depth_coeff

    def generate(self, vertex_count):
        root = self.create_node()
        vertices = [root]
        weights = [1]

        vertex_count -= 1
        while vertex_count > 0:
            parent = random.choices(vertices, weights=weights)[0]

            node = self.create_node()
            self.add_node(parent, node)
            vertices.append(node)
            weights.append(self._depth_coeff * node.depth)
            vertex_count -= 1

            if not self.can_add_node(parent):
                index = vertices.index(parent)
                vertices = vertices[:index] + vertices[index + 1:]
                weights = weights[:index] + weights[index + 1:]
                continue

        return root
