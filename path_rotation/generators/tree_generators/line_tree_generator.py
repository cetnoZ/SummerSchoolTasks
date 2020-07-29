import random
from .tree_generator import *


class LineTreeGenerator(TreeGenerator):
    def __init__(self):
        TreeGenerator.__init__(self)

    def generate(self, vertex_count):
        root = self.create_node()
        vertex = root

        for _ in range(vertex_count - 1):
            node = self.create_node()
            direction = random.choice(['left', 'right'])
            if direction == 'left':
                vertex.left_node = node
                vertex = node
            elif direction == 'right':
                vertex.right_node = node
                vertex = node
        return root
