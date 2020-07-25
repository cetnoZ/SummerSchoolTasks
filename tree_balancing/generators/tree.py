import sys
import random
from collections import deque


class Node:
    def __init__(self, index):
        self._index = index
        self._left_node = None
        self._right_node = None
        self._depth = 1
        self._weight = None
        self._parent = None

    @property
    def index(self):
        return self._index

    @property
    def depth(self):
        return self._depth

    @property
    def parent(self):
        return self._parent

    @property
    def weight(self):
        return self._weight

    @weight.setter
    def weight(self, weight):
        self._weight = weight

    @property
    def left_node(self):
        return self._left_node

    @property
    def left_index(self):
        return self._left_node.index if self._left_node is not None else 0

    @left_node.setter
    def left_node(self, left_node):
        left_node._depth = self._depth + 1
        left_node._parent = self
        self._left_node = left_node

    @property
    def right_node(self):
        return self._right_node

    @property
    def right_index(self):
        return self._right_node.index if self._right_node is not None else 0

    @right_node.setter
    def right_node(self, right_node):
        right_node._depth = self._depth + 1
        right_node._parent = self
        self._right_node = right_node

    @property
    def children(self):
        return [vertex for vertex in [self.left_node, self.right_node] if vertex is not None]

def tree_nodes(node):
    vertices = deque([node])
    while len(vertices) > 0:
        vertex = vertices.pop()
        yield vertex

        for child in vertex.children:
            vertices.appendleft(child)

def count_vertices(node):
    return len(tree_nodes(node))

def print_tree(root, file=sys.stdout):
    vertices = list(tree_nodes(root))

    print(len(vertices), file=file)

    vertices.sort(key=lambda vertex: vertex.index)

    for node in vertices:
        print(f"{node.left_index} {node.right_index} {node.weight}", file=file)
