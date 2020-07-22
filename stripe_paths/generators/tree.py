import sys
import random
from collections import deque


class Node:
    def __init__(self, index, label=None):
        self._index = index
        self._label = label
        self._children = []
        self._depth = 1
        self._parent = None

    @property
    def index(self):
        return self._index

    @property
    def children(self):
        return self._children

    @property
    def depth(self):
        return self._depth

    @property
    def parent(self):
        return self._parent

    @property
    def label(self):
        return self._label

    @label.setter
    def label(self, label):
        self._label = label

    def add_child(self, node):
        node._depth = self._depth + 1
        node._parent = self
        self._children.append(node)

def tree_to_edges(node):
    vertices = deque([node])
    while len(vertices) > 0:
        vertex = vertices.pop()
        for child in vertex.children:
            vertices.appendleft(child)
            yield (vertex.index, child.index, child.label)

def count_vertices(node):
    count = 1
    vertices = deque([node])
    while len(vertices) > 0:
        vertex = vertices.pop()
        for child in vertex.children:
            vertices.appendleft(child)
            count += 1
    return count

def print_tree(node, file=sys.stdout):
    n = count_vertices(node)
    edges = list(tree_to_edges(node))

    print(n, file=file)

    random.shuffle(edges)
    for (v, u, label) in edges:
        print(f"{v} {u} {label}", file=file)
