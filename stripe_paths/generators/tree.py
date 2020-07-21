import sys
import random


class Node:
    def __init__(self, index, label=None):
        self._index = index
        self._label = label
        self._children = []

    @property
    def index(self):
        return self._index

    @property
    def children(self):
        return self._children
    @property
    def label(self):
        return self._label

    @label.setter
    def label(self, label):
        self._label = label

    def add_child(self, node):
        self._children.append(node)

def tree_to_edges(node):
    for child in node.children:
        yield (node.index, child.index, child.label)
        for child_edge in tree_to_edges(child):
            yield child_edge

def count_vertices(node):
    count = 1
    for child in node.children:
        count += count_vertices(child)
    return count

def print_tree(node, file=sys.stdout):
    n = count_vertices(node)
    edges = list(tree_to_edges(node))

    print(n, file=file)

    random.shuffle(edges)
    for (v, u, label) in edges:
        print(f"{v} {u} {label}", file=file)
