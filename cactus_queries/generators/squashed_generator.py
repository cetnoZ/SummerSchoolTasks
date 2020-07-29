import os
import sys
import random
from collections import deque

################# graph.py #########################

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




################# tree_expander.py #########################

class TreeExpander:
    def expand_tree(self, tree, expected_size):
        return tree




################# random_tree_expander.py #########################

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




################# tree_generator.py #########################

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




################# random_tree_generator.py #########################

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




################# depth_tree_generator.py #########################

class DepthTreeGenerator(TreeGenerator):
    def __init__(self, depth_coeff=1):
        TreeGenerator.__init__(self)
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

class LineTreeGenerator(TreeGenerator):
    def __init__(self, depth_coeff=1):
        TreeGenerator.__init__(self)
        self._depth_coeff = depth_coeff

    def generate(self, vertex_count):
        root = self.create_node()
        vertex = root

        for _ in range(vertex_count - 1):
            vertex, parent = self.create_node(), vertex
            parent.add_child(vertex)

        return root




################# generator.py #########################

def create_tree_generator(generator_type, argv):
    if generator_type == 'random':
        return RandomTreeGenerator()
    if generator_type == 'line':
        return LineTreeGenerator()
    if generator_type == 'depth':
        depth_coeff = float(argv.get('DEPTH_COEFF', '1'))
        return DepthTreeGenerator(depth_coeff)

def create_tree_expander(average_cycle, cycle_variation):
    return RandomTreeExpander(average_cycle, cycle_variation)

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

def main(args):
    test_index = int(args.get('TEST_INDEX', '0')) + 1
    test_dir = args.get('TEST_DIR')

    seed_value = args.get('SEED')
    tree_type = args.get('TREE_TYPE', 'random')
    average_cycle = int(args.get('AVERAGE_CYCLE', '10'))
    cycle_variation = float(args.get('CYCLE_VARIATION', '0.1'))
    vertex_count = int(args.get('VERTEX_COUNT', '1'))
    queries_count = int(args.get('QUERIES_COUNT', '1'))

    random.seed(f"{seed_value}_{test_index}")

    tree_generator = create_tree_generator(tree_type, args)
    tree_expander = create_tree_expander(average_cycle, cycle_variation)

    current_vertex_count = random.randint(max(3, vertex_count - max(5, vertex_count // 10)), vertex_count)

    tree_vertex_count = random.randint(max(1, current_vertex_count // average_cycle), max(1, current_vertex_count // 2))

    tree = tree_generator.generate(tree_vertex_count)
    result_graph = tree_expander.expand_tree(tree, current_vertex_count)

    print_graph(result_graph)
    current_queries_count = random.randint(0, queries_count)
    queries = list(generate_queries(result_graph, current_queries_count))
    print_queries(queries)





TEST_INDEX, SEED, VERTEX_COUNT, QUERIES_COUNT, TREE_TYPE, AVERAGE_CYCLE, CYCLE_VARIATIONS, DEPTH_COEFF = (sys.argv[1:] + [None, None])[:8]


TEST_INDEX       = sys.argv[1] if len(sys.argv) > 1 else None
SEED             = sys.argv[2] if len(sys.argv) > 2 else None
VERTEX_COUNT     = sys.argv[3] if len(sys.argv) > 3 else None
QUERIES_COUNT    = sys.argv[4] if len(sys.argv) > 4 else None
TREE_TYPE        = sys.argv[5] if len(sys.argv) > 5 else None
AVERAGE_CYCLE    = sys.argv[6] if len(sys.argv) > 6 else None
CYCLE_VARIATIONS = sys.argv[7] if len(sys.argv) > 7 else None
DEPTH_COEFF      = sys.argv[8] if len(sys.argv) > 8 else None

main({ 'SEED': SEED, 'VERTEX_COUNT': VERTEX_COUNT, 'QUERIES_COUNT': QUERIES_COUNT, 'TREE_TYPE': TREE_TYPE, 'TEST_INDEX': TEST_INDEX, 'AVERAGE_CYCLE': AVERAGE_CYCLE, 'CYCLE_VARIATIONS': CYCLE_VARIATIONS, 'DEPTH_COEFF': DEPTH_COEFF })
