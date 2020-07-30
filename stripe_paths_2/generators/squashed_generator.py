import os
import sys
import random
from math import *
from collections import deque


sys.setrecursionlimit(10 ** 6)




#################### tree.py ######################

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
    return len(list(tree_nodes(node)))

def print_tree(root, file=sys.stdout):
    vertices = list(tree_nodes(root))

    print(len(vertices), file=file)

    vertices.sort(key=lambda vertex: vertex.index)

    for node in vertices:
        print(f"{node.left_index} {node.right_index}", file=file)






#################### query_generator.py ######################

class QueryGenerator:
    def __init__(self):
        self._K = 20

    def generate_queries(self, root, count):
        self._vertex_count = count_vertices(root)

        self._parents = [[0 for _ in range(self._vertex_count + 1)] for _ in range(self._K + 1)]
        self._ltime = [0] * (self._vertex_count + 1)
        self._rtime = [0] * (self._vertex_count + 1)
        self._time = 0

        self._vertices = {node.index: node for node in tree_nodes(root)}

        self.precalc_lca(root)
        return self.generate_all_queries(root, count)

    def generate_all_queries(self, root, count):
        pass

    def inside(self, higher, lower):
        return self._ltime[higher] <= self._ltime[lower] and self._rtime[lower] <= self._rtime[higher]

    def precalc_lca(self, node, parent=None):
        if node is None:
            return

        self._time += 1
        self._ltime[node.index] = self._time

        self._parents[0][node.index] = parent.index if parent is not None else 0
        for j in range(1, self._K):
            self._parents[j][node.index] = self._parents[j - 1][self._parents[j - 1][node.index]]

        self.precalc_lca(node.left_node, node)
        self.precalc_lca(node.right_node, node)

        self._rtime[node.index] = self._time

    def lca(self, u, v):
        if self.inside(u, v):
            return u
        if self.inside(v, u):
            return v
        
        for j in range(self._K - 1, -1, -1):
            upper = self._parents[j][v]
            if upper == 0 or self.inside(upper, u):
                continue
            v = upper
        return self._parents[0][v]

    def find_divisors(self, k):
        for i in range(1, round(sqrt(k)) + 1):
            if k % i == 0:
                if (k // i) * (k // i) == k:
                    yield i
                else:
                    yield i
                    yield k // i






#################### random_query_generator.py ######################

class RandomQueryGenerator(QueryGenerator):
    def __init__(self, percent):
        QueryGenerator.__init__(self)
        self._percent = percent

    def generate_ask_query(self, vertex_count):
        u, v = random.sample(range(1, vertex_count + 1), 2)
        return u, v

    def generate_modify_query(self, vertex_count):
        for _ in range(5):
            u, v = random.sample(range(1, vertex_count + 1), 2)
            lca_index = self.lca(u, v)

            path_length = self._vertices[u].depth + self._vertices[v].depth - 2 * self._vertices[lca_index].depth
            if path_length % 2 != 0:
                continue

            k = random.choice(list(self.find_divisors(path_length // 2)))

            return u, v, k

    def generate_all_queries(self, root, count):
        for _ in range(count):
            if random.random() > self._percent:
                u, v = self.generate_ask_query(self._vertex_count)
                yield (1, u, v)
            else:
                query = self.generate_modify_query(self._vertex_count)
                if query is not None:
                    u, v, k = query
                    yield (2, u, v, k)





#################### tree_generator.py ######################

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





#################### depth_tree_generator.py ######################

class DepthTreeGenerator(TreeGenerator):
    def __init__(self, depth_coeff=1):
        TreeGenerator.__init__(self)
        self._depth_coeff = depth_coeff

    def generate(self, vertex_count):
        root = self.create_node()
        vertices = [root]
        weights = [1]

        vertex_count -= 1
        while vertex_count > 0:
            parent = random.choices(vertices, weights=weights)[0]

            node = self.create_node()
            self.add_node(parent, node)
            vertices.append(node)
            weights.append(self._depth_coeff * node.depth)
            vertex_count -= 1

            if not self.can_add_node(parent):
                index = vertices.index(parent)
                vertices = vertices[:index] + vertices[index + 1:]
                weights = weights[:index] + weights[index + 1:]
                continue

        return root





#################### line_tree_generator.py ######################

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





#################### random_tree_generator.py ######################

class RandomTreeGenerator(TreeGenerator):
    def __init__(self):
        TreeGenerator.__init__(self)

    def generate(self, vertex_count):
        root = self.create_node()
        vertices = [root]

        vertex_count -= 1
        while vertex_count > 0:
            parent = random.choice(vertices)

            node = self.create_node()
            self.add_node(parent, node)
            vertices.append(node)
            vertex_count -= 1

            if not self.can_add_node(parent):
                vertices.remove(parent)
                continue
        return root





#################### generator.py ######################

def create_tree_generator(generator_type, args):
    if generator_type == 'random':
        return RandomTreeGenerator()
    if generator_type == 'line':
        return LineTreeGenerator()
    if generator_type == 'depth':
        depth_coeff = float(args.get('DEPTH_COEFF', '1'))
        return DepthTreeGenerator(depth_coeff)

def create_query_generator(query_type, args):
    if query_type == 'random':
        query_percent = float(args.get('QUERY_PERCENT', '0.1'))
        return RandomQueryGenerator(query_percent)

def generate_queries(tree, queries_count):
    nodes_count = len(list(tree_nodes(tree)))

    for _ in range(queries_count):
        first_index, second_index = random.sample(range(1, nodes_count + 1), 2)
        yield (first_index, second_index)

def print_queries(queries, file=sys.stdout):
    queries_count = len(queries)
    print(queries_count, file=file)

    for query in queries:
        print(' '.join(map(str, query)), file=file)

def main(args):

    test_index = int(args.get('TEST_INDEX', '0')) + 1

    seed_value = args.get('SEED')
    vertex_count = int(args.get('VERTEX_COUNT', '1'))
    queries_count = int(args.get('QUERIES_COUNT', '1'))
    tree_type = args.get('TREE_TYPE', 'random')
    query_type = args.get('QUERY_TYPE', 'random')

    random.seed(f"{seed_value}_{test_index}")

    tree_generator = create_tree_generator(tree_type, args)
    query_generator = create_query_generator(query_type, args)
    vertex_count_delta = min(vertex_count - 1, max(vertex_count // 10, 5))

    tree = tree_generator.generate(vertex_count + random.randint(-vertex_count_delta, 0))
    print_tree(tree)

    queries = query_generator.generate_queries(tree, random.randint(queries_count - queries_count // 10, queries_count))
    print_queries(list(queries))


TEST_INDEX      = sys.argv[1] if len(sys.argv) > 1 else None
SEED            = sys.argv[2] if len(sys.argv) > 2 else None
VERTEX_COUNT    = sys.argv[3] if len(sys.argv) > 3 else None
QUERIES_COUNT   = sys.argv[4] if len(sys.argv) > 4 else None
TREE_TYPE       = sys.argv[5] if len(sys.argv) > 5 else None
QUERY_TYPE      = sys.argv[6] if len(sys.argv) > 6 else None
QUERY_PERCENT   = sys.argv[7] if len(sys.argv) > 7 else None
DEPTH_COEFF     = sys.argv[8] if len(sys.argv) > 8 else None


main({
    'TEST_INDEX':     TEST_INDEX,
    'SEED':           SEED,
    'VERTEX_COUNT':   VERTEX_COUNT,
    'QUERIES_COUNT':  QUERIES_COUNT,
    'TREE_TYPE':      TREE_TYPE,
    'QUERY_TYPE':     QUERY_TYPE,
    'QUERY_PERCENT':  QUERY_PERCENT,
    'DEPTH_COEFF':    DEPTH_COEFF,
    })
