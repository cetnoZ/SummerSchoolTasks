import random
from graph import * 
from .tree_expander import *


class RandomTreeExpander(TreeExpander):
    def __init__(self, average_cycle, cycle_variation):
        TreeExpander.__init__(self)
        self._average_cycle = average_cycle
        self._cycle_variation = cycle_variation
        self._node_counter = 0

    def expand_tree(self, tree, expected_size):
        nodes = list(graph_to_nodes(tree))
        nodes_count = len(nodes)
        node_weight = {}

        random.shuffle(nodes)

        for node in nodes:
            weight = self.pick_node_weight(expected_size - nodes_count + 1)
            nodes_count += weight - 1

            node_weight[node.index] = weight

        self._node_counter = 0
        cycles = {node.index: self.create_cycle(node_weight.get(node.index, 1)) for node in graph_to_nodes(tree)}

        for (first_node, second_node) in graph_to_edges(tree):
            first_cycle = cycles[first_node.index]
            second_cycle = cycles[second_node.index]

            first_cycle_node = random.choice(first_cycle)
            second_cycle_node = random.choice(second_cycle)

            first_cycle_node.add_neighbor(second_cycle_node)
        return list(cycles.values())[0][0]

    def pick_node_weight(self, max_weight):
        max_weight = min(max_weight, int(self._average_cycle * (1 + self._cycle_variation)))
        min_weight = max(1, max_weight - 2 * int(self._average_cycle * (1 - self._cycle_variation)))
        return random.randint(min_weight, max_weight)

    def create_node(self):
        self._node_counter += 1
        return Node(self._node_counter)

    def create_cycle(self, weight):
        cycle_nodes = [self.create_node() for _ in range(weight)]
        if weight > 2:
            for i in range(weight):
                cycle_nodes[i].add_neighbor(cycle_nodes[i - 1])
        elif weight == 2:
            cycle_nodes[0].add_neighbor(cycle_nodes[1])

        return cycle_nodes
