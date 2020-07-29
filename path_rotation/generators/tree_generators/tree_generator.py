import random
from tree import *


class TreeGenerator:
    def __init__(self):
        self.vertex_counter = 0

    def generate(self, vertex_count):
        return None

    def add_node(self, parent, node):
        if (parent.left_node is not None) and (parent.right_node is not None):
            return False
        elif (parent.left_node is None) and (parent.right_node is None):
            if random.randint(0, 1) == 0:
                parent.left_node = node
            else:
                parent.right_node = node
        elif parent.left_node is None:
            parent.left_node = node
        elif parent.right_node is None:
            parent.right_node = node
        return True

    def can_add_node(self, parent):
        return (parent.left_node is None) or (parent.right_node is None)

    def create_node(self):
        self.vertex_counter += 1
        return Node(self.vertex_counter)

    create_node.counter = 0
