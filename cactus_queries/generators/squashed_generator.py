import os
import sys
import random
from collections import deque

################# graph.py #########################

class node:
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




################# tree_expander.py #########################

class treeexpander:
    def expand_tree(self, tree, expected_size):
        return tree




################# random_tree_expander.py #########################

class randomtreeexpander(treeexpander):
    def __init__(self, average_cycle, cycle_variation):
        treeexpander.__init__(self)
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
        return node(self._node_counter)

    def create_cycle(self, weight):
        cycle_nodes = [self.create_node() for _ in range(weight)]
        if weight > 2:
            for i in range(weight):
                cycle_nodes[i].add_neighbor(cycle_nodes[i - 1])
        elif weight == 2:
            cycle_nodes[0].add_neighbor(cycle_nodes[1])

        return cycle_nodes




################# tree_generator.py #########################

class treegenerator:
    def __init__(self):
        self.vertex_counter = 0

    def generate(self, vertex_count):
        return none

    def add_node(self, parent, node):
        if (parent.left_node is not none) and (parent.right_node is not none):
            return false
        elif (parent.left_node is none) and (parent.right_node is none):
            if random.randint(0, 1) == 0:
                parent.left_node = node
            else:
                parent.right_node = node
        elif parent.left_node is none:
            parent.left_node = node
        elif parent.right_node is none:
            parent.right_node = node
        return true

    def can_add_node(self, parent):
        return (parent.left_node is none) or (parent.right_node is none)

    def create_node(self):
        self.vertex_counter += 1
        return node(self.vertex_counter)

    create_node.counter = 0




################# random_tree_generator.py #########################

class randomtreegenerator(treegenerator):
    def __init__(self):
        treegenerator.__init__(self)

    def generate(self, vertex_count):
        root = self.create_node()
        vertices = [root]

        for _ in range(vertex_count - 1):
            node = self.create_node()

            parent = random.choice(vertices)
            parent.add_child(node)

            vertices.append(node)
        return root




################# depth_tree_generator.py #########################

class depthtreegenerator(treegenerator):
    def __init__(self, depth_coeff=1):
        treegenerator.__init__(self)
        self._depth_coeff = depth_coeff

    def generate(self, vertex_count):
        root = self.create_node()
        vertices = [root]
        accumulated_weights = [1]

        for _ in range(vertex_count - 1):
            node = self.create_node()
            parent = random.choices(vertices, cum_weights=accumulated_weights)[0]

            parent.add_child(node)
            vertices.append(node)
            accumulated_weights.append(accumulated_weights[-1] + self._depth_coeff * node.depth)
        return root




################# line_tree_generator.py #########################

class linetreegenerator(treegenerator):
    def __init__(self, depth_coeff=1):
        treegenerator.__init__(self)
        self._depth_coeff = depth_coeff

    def generate(self, vertex_count):
        root = self.create_node()
        vertex = root

        for _ in range(vertex_count - 1):
            vertex, parent = self.create_node(), vertex
            parent.add_child(vertex)

        return root




################# generator.py #########################

def create_tree_generator(generator_type='random'):
    if generator_type == 'random':
        return randomtreegenerator()
    if generator_type == 'line':
        return linetreegenerator()
    if generator_type == 'depth':
        depth_coeff = float(os.getenv('depth_coeff', '1'))
        return depthtreegenerator(depth_coeff)

def create_tree_expander(average_cycle, cycle_variation):
    return randomtreeexpander(average_cycle, cycle_variation)

def generate_queries(graph, queries_count):
    nodes_count = len(list(graph_to_nodes(graph)))
    edges = {(first_node.index, second_node.index) for (first_node, second_node) in graph_to_edges(graph)}

    if nodes_count > 3:
        for _ in range(queries_count):
            first_index, second_index = random.sample(range(1, nodes_count + 1), 2)
            while ((first_index, second_index) in edges) or ((second_index, first_index) in edges):
                first_index, second_index = random.sample(range(1, nodes_count + 1), 2)
            yield (first_index, second_index)


def print_queries(queries, file=sys.stdout):
    print(len(queries), file=file)
    for (first_index, second_index) in queries:
        print(f"{first_index} {second_index}", file=file)

def main():
    test_index = int(os.getenv('test_index', '0')) + 1
    test_dir = os.getenv('test_dir')

    seed_value = os.getenv('seed')
    tree_type = os.getenv('tree_type', 'random')
    average_cycle = int(os.getenv('average_cycle', '10'))
    cycle_variation = float(os.getenv('cycle_variation', '0.1'))
    vertex_count = int(os.getenv('vertex_count', '1'))
    queries_count = int(os.getenv('queries_count', '1'))

    random.seed(f"{seed_value}_{test_index}")

    tree_generator = create_tree_generator(tree_type)
    tree_expander = create_tree_expander(average_cycle, cycle_variation)

    current_vertex_count = random.randint(max(3, vertex_count - max(5, vertex_count // 10)), vertex_count)

    tree_vertex_count = random.randint(max(1, current_vertex_count // average_cycle), max(1, current_vertex_count // 2))

    tree = tree_generator.generate(tree_vertex_count)
    result_graph = tree_expander.expand_tree(tree, current_vertex_count)

    print_graph(result_graph)
    current_queries_count = random.randint(0, queries_count)
    queries = list(generate_queries(result_graph, current_queries_count))
    print_queries(queries)

def generate_tests(args):
    for key in args:
        os.environ[key] = str(args[key])
    main()

if __name__ == '__main__':
    test_index, seed, vertex_count, queries_count, tree_type, average_cycle, cycle_variations, depth_coeff = sys.argv[1:]

    generate_tests({ 'seed': seed, 'vertex_count': vertex_count, 'queries_count': queries_count, 'tree_type': tree_type, 'test_index': test_index, 'average_cycle': average_cycle, 'cycle_variations': cycle_variations, 'depth_coeff': depth_coeff })
