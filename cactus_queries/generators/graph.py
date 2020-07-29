import sys
import random
from collections import deque


class Node:
    def __init__(self, index):
        self._index = index
        self._neighbors = []
        self._depth = 1

    @property
    def index(self):
        return self._index

    @property
    def neighbors(self):
        return self._neighbors

    @property
    def depth(self):
        return self._depth

    def add_child(self, node):
        node._depth = self._depth + 1
        self._neighbors.append(node)
        node._neighbors.append(self)

    def add_neighbor(self, node):
        self._neighbors.append(node)
        node._neighbors.append(self)

    def __repr__(self):
        return f"{self.index}"

def graph_to_edges(node):
    used = set([node.index])
    edges = set()

    nodes = deque([node])
    while len(nodes) > 0:
        current_node = nodes.pop()

        for neighbor in current_node.neighbors:
            if ((current_node.index, neighbor.index) not in edges) and ((neighbor.index, current_node.index) not in edges):
                edges.add((current_node.index, neighbor.index))
                yield (current_node, neighbor)
            if neighbor.index not in used:
                nodes.appendleft(neighbor)
                used.add(neighbor.index)

def graph_to_nodes(node):
    used = set([node.index])

    nodes = deque([node])
    while len(nodes) > 0:
        current_node = nodes.pop()
        yield current_node

        for neighbor in current_node.neighbors:
            if neighbor.index not in used:
                nodes.appendleft(neighbor)
                used.add(neighbor.index)

def print_graph(node, file=sys.stdout):
    nodes = list(graph_to_nodes(node))
    edges = list(graph_to_edges(node))

    print(f"{len(nodes)} {len(edges)}", file=file)

    random.shuffle(edges)
    for (first_node, second_node) in edges:
        print(f"{first_node} {second_node}", file=file)
