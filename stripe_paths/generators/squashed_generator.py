import os
import sys
import math
import random
import itertools
from collections import deque





################### tree.py #################

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





################### tree.py #################

class TreeGenerator:
    def __init__(self):
        self.vertex_counter = 0

    def generate(self, vertex_count):
        return None

    def create_node(self):
        self.vertex_counter += 1
        return Node(self.vertex_counter)

    create_node.counter = 0





################### depth_tree_generator.py #################

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





################### line_tree_generator.py #################

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






################### random_tree_generator.py #################

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






################### tree_labeler.py #################

class TreeLabeler:
    def label_tree(self, root):
        return root

    def calc_depths(self, root):
        depths = {}
        vertices = deque([root])
        while len(vertices) > 0:
            vertex = vertices.pop()
            depth = vertex.depth - 1

            if depth in depths:
                depths[depth] += [vertex]
            else:
                depths[depth] = [vertex]

            for child in vertex.children:
                vertices.appendleft(child)
        return depths

    def label_up(self, vertex, k, up_cnt):
        for _ in range(up_cnt):
            if vertex == None:
                break

            for _ in range(k):
                if vertex == None:
                    break
                vertex.label = 0
                vertex = vertex.parent

            for _ in range(k):
                if vertex == None:
                    break
                vertex.label = 1
                vertex = vertex.parent






################### random_tree_labeler.py #################

class RandomTreeLabeler(TreeLabeler):
    def __init__(self, percent):
        TreeLabeler.__init__(self)
        self._percent = percent

    def label_tree(self, root):
        vertices = deque([root])
        while len(vertices) > 0:
            vertex = vertices.pop()
            vertex.label = 1 if random.random() > self._percent else 0
            for child in vertex.children:
                vertices.appendleft(child)
        return root







################### sripes_tree_labeler.py #################

class StripesTreeLabeler(TreeLabeler):
    def __init__(self, stripes_count, k_type):
        TreeLabeler.__init__(self)
        self._random_labeler = RandomTreeLabeler(0.5)
        self._k_type = k_type
        self._stripes_count = stripes_count

    def label_tree(self, root):
        self._random_labeler.label_tree(root)

        depths = self.calc_depths(root)
        depths_keys = list(depths.keys())
        weights = list(map(len, depths.values()))
        accumulated_weights = list(itertools.accumulate(weights))
        depth = max(depths_keys)

        for _ in range(self._stripes_count):
            k = self.generate_k(self._k_type, depth)
            vertex = self.pick_vertex(k, depths, depths_keys, accumulated_weights)
            up_cnt = self.pick_up_cnt(k, vertex)
            self.label_up(vertex, k, up_cnt)

        return root

    def generate_k(self, k_type, depth):
        if k_type == 'average':
            median_k = round(math.sqrt(depth))
        elif k_type == 'big':
            median_k = round(depth / random.randint(2, 10))
        elif k_type == 'small':
            median_k = random.randint(1, 10)
        elif k_type == 'random':
            return self.generate_k(random.choice(['average', 'big', 'small']), depth)

        median_k = min(depth // 2, max(1, median_k))
        delta_k = max(2, median_k // 10)

        min_k = max(1, median_k - delta_k)
        max_k = max(min_k, min(depth // 2, median_k + delta_k))

        k = random.randint(min_k, max_k)

        return k

    def pick_vertex(self, k, depths, depths_keys, accumulated_weights):
        depth = random.choices(depths_keys[2 * k - 1:], cum_weights=accumulated_weights[2 * k - 1:])[0]
        vertex = random.choice(depths[depth])
        return vertex

    def pick_up_cnt(self, k, vertex):
        min_up_cnt = 1
        max_up_cnt = vertex.depth // (2 * k)
        return random.randint(min_up_cnt, max_up_cnt)







################### one_sripe_tree_labeler.py #################

class OneStripeTreeLabeler(TreeLabeler):
    def __init__(self, k_type):
        TreeLabeler.__init__(self)
        self._random_labeler = RandomTreeLabeler(0.5)
        self._k_type = k_type

    def label_tree(self, root):
        self._random_labeler.label_tree(root)

        depths = self.calc_depths(root)
        depths_keys = list(depths.keys())
        weights = list(map(len, depths.values()))
        accumulated_weights = list(itertools.accumulate(weights))
        depth = max(depths_keys)

        k = self.generate_k(self._k_type, depth)
        vertex = self.pick_vertex(k, depths, depths_keys, accumulated_weights)
        up_cnt = self.pick_up_cnt(k, vertex)
        self.label_up(vertex, k, up_cnt)

        return root

    def generate_k(self, k_type, depth):
        if k_type == 'average':
            median_k = round(math.sqrt(depth))
        elif k_type == 'big':
            median_k = round(depth / random.randint(2, 10))
        elif k_type == 'small':
            return random.randint(1, 5)
        elif k_type == 'little':
            return random.randint(1, 2)
        elif k_type == 'random':
            return self.generate_k(random.choice(['average', 'big', 'small']), depth)

        median_k = min(depth // 2, max(1, median_k))
        delta_k = max(2, median_k // 10)

        min_k = max(1, median_k - delta_k)
        max_k = max(min_k, min(depth // 2, median_k + delta_k))

        k = random.randint(min_k, max_k)

        return k

    def pick_vertex(self, k, depths, depths_keys, accumulated_weights):
        depth = depths_keys[-1]
        vertex = random.choice(depths[depth])
        return vertex

    def pick_up_cnt(self, k, vertex):
        return vertex.depth // (2 * k)






################### generator.py #################

def create_tree_generator(generator_type, args):
    if generator_type == 'random':
        return RandomTreeGenerator()
    if generator_type == 'line':
        return LineTreeGenerator()
    elif generator_type == 'depth':
        depth_coeff = float(args.get('DEPTH_COEFF', '1'))
        return DepthTreeGenerator(depth_coeff)

def create_tree_labeler(labeler_type, args):
    if labeler_type == 'random':
        percent = float(args.get('LABEL_PERCENT', '0.5'))
        return RandomTreeLabeler(percent)
    elif labeler_type == 'stripes':
        stripes_count = int(args.get('STRIPES_COUNT', '10'))
        k_type = args.get('K_TYPE', 'random')

        return StripesTreeLabeler(stripes_count, k_type)
    elif labeler_type == 'one_stripe':
        k_type = args.get('K_TYPE', 'random')

        return OneStripeTreeLabeler(k_type)

def main(args):
    test_index = int(args.get('TEST_INDEX', '0')) + 1
    seed_value = args.get('SEED')
    vertex_count = int(args.get('VERTEX_COUNT', '1'))
    tree_type = args.get('TREE_TYPE', 'random')
    label_type = args.get('LABEL_TYPE', 'random')

    random.seed(f"{seed_value}_{test_index}")

    tree_generator = create_tree_generator(tree_type, args)
    tree_labeler = create_tree_labeler(label_type, args)
    vertex_count_delta = min(vertex_count - 1, max(vertex_count // 10, 5))

    tree = tree_generator.generate(vertex_count + random.randint(-vertex_count_delta, 0))
    tree_labeler.label_tree(tree)
    print_tree(tree)


TEST_INDEX     = sys.argv[1] if len(sys.argv) > 1 else None
SEED           = sys.argv[2] if len(sys.argv) > 2 else None
VERTEX_COUNT   = sys.argv[3] if len(sys.argv) > 3 else None
TREE_TYPE      = sys.argv[4] if len(sys.argv) > 4 else None
LABEL_TYPE     = sys.argv[5] if len(sys.argv) > 5 else None
LABEL_PERCENT  = sys.argv[6] if len(sys.argv) > 6 else None
DEPTH_COEFF    = sys.argv[7] if len(sys.argv) > 7 else None
K_TYPE         = sys.argv[8] if len(sys.argv) > 8 else None
STRIPES_COUNT  = sys.argv[9] if len(sys.argv) > 9 else None


main({
    'TEST_INDEX':     TEST_INDEX,
    'SEED':           SEED,
    'VERTEX_COUNT':   VERTEX_COUNT,
    'TREE_TYPE':      TREE_TYPE,
    'LABEL_TYPE':     LABEL_TYPE,
    'LABEL_PERCENT':  LABEL_PERCENT,
    'DEPTH_COEFF':    DEPTH_COEFF,
    'K_TYPE':         K_TYPE,
    'STRIPES_COUNT':  STRIPES_COUNT,
    })
