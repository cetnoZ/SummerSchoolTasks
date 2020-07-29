import random
from .tree_generator import *


class RandomTreeGenerator(TreeGenerator):
    def __init__(self):
        TreeGenerator.__init__(self)

    def generate(self, vertex_count):
        root = self.create_node()
        vertices = [root]

        vertex_count -= 1
        while vertex_count > 0:
            parent = random.choice(vertices)

            node = self.create_node()
            self.add_node(parent, node)
            vertices.append(node)
            vertex_count -= 1

            if not self.can_add_node(parent):
                vertices.remove(parent)
                continue
        return root
