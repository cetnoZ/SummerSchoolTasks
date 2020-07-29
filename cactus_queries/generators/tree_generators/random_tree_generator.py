import random
from .tree_generator import *


class RandomTreeGenerator(TreeGenerator):
    def __init__(self):
        TreeGenerator.__init__(self)

    def generate(self, vertex_count):
        root = self.create_node()
        vertices = [root]

        for _ in range(vertex_count - 1):
            node = self.create_node()

            parent = random.choice(vertices)
            parent.add_child(node)

            vertices.append(node)
        return root
