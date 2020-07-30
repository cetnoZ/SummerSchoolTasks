import os
import sys
import random
from collections import deque

################# tree.py #########################

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

        return cycle_node



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




################# depth_tree_generator.py #########################

def create_tree_generator(generator_type, args):
    if generator_type == 'random':
        return RandomTreeGenerator()
    if generator_type == 'line':
        return LineTreeGenerator()
    if generator_type == 'depth':
        depth_coeff = float(args.get('DEPTH_COEFF', '1'))
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





TEST_INDEX       = sys.argv[1] if len(sys.argv) > 1 else None
SEED             = sys.argv[2] if len(sys.argv) > 2 else None
VERTEX_COUNT     = sys.argv[3] if len(sys.argv) > 3 else None
QUERIES_COUNT    = sys.argv[4] if len(sys.argv) > 4 else None
TREE_TYPE        = sys.argv[5] if len(sys.argv) > 5 else None
WEIGHT_TYPE      = sys.argv[6] if len(sys.argv) > 6 else None
MAX_WEIGHT       = sys.argv[7] if len(sys.argv) > 7 else None
WEIGHT_PERCENT   = sys.argv[8] if len(sys.argv) > 8 else None
DEPTH_COEFF      = sys.argv[9] if len(sys.argv) > 9 else None

main({
    'TEST_INDEX': TEST_INDEX,
    'SEED': SEED,
    'VERTEX_COUNT': VERTEX_COUNT,
    'QUERIES_COUNT': QUERIES_COUNT,
    'TREE_TYPE': TREE_TYPE,
    'WEIGHT_TYPE': WEIGHT_TYPE,
    'MAX_WEIGHT': MAX_WEIGHT,
    'WEIGHT_PERCENT': WEIGHT_PERCENT,
    'DEPTH_COEFF': DEPTH_COEFF
    })
