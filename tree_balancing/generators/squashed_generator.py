import random
import math
import sys
from collections import deque




############## tree.py ###############

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





############## tree_generator.py ###############

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





############## random_tree_generator.py ###############

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





############## line_tree_generator.py ###############

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







############## depth_tree_generator.py ###############

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






############## tree_weighter.py ###############

class TreeWeighter:
    def set_weights(self, root):
        return root





############## random_tree_weighter.py ###############

class RandomTreeWeighter(TreeWeighter):
    def __init__(self, max_weight, percent=0.1):
        TreeWeighter.__init__(self)
        self._max_weight = max_weight
        self._percent = percent

    def set_weights(self, root):
        for vertex in tree_nodes(root):
            vertex.weight = self.pick_weight()
        return root

    def pick_weight(self):
        min_weight = max(0, round(self._max_weight * (1 - self._percent)))
        max_weight = self._max_weight
        return random.randint(min_weight, max_weight)






############## generator.py ###############

def create_tree_generator(generator_type, args):
    if generator_type == 'random':
        return RandomTreeGenerator()
    if generator_type == 'line':
        return LineTreeGenerator()
    if generator_type == 'depth':
        depth_coeff = float(args.get('DEPTH_COEFF', '1'))
        return DepthTreeGenerator(depth_coeff)

def create_tree_weighter(weight_type, args):
    if weight_type == 'random':
        max_weight = float(args.get('MAX_WEIGHT'))
        weight_percent = float(args.get('WEIGHT_PERCENT', '0.1'))
        return RandomTreeWeighter(max_weight, weight_percent)

def main(args):
    test_index = int(args.get('TEST_INDEX', '0')) + 1

    seed_value = args.get('SEED')
    vertex_count = int(args.get('VERTEX_COUNT', '1'))
    tree_type = args.get('TREE_TYPE', 'random')
    weight_type = args.get('WEIGHT_TYPE', 'random')

    random.seed(f"{seed_value}_{test_index}")

    tree_generator = create_tree_generator(tree_type, args)
    tree_weighter = create_tree_weighter(weight_type, args)
    vertex_count_delta = min(vertex_count - 1, max(vertex_count // 10, 5))

    tree = tree_generator.generate(vertex_count + random.randint(-vertex_count_delta, 0))
    tree = tree_weighter.set_weights(tree)
    print_tree(tree)


TEST_INDEX        = sys.argv[1] if len(sys.argv) > 1 else None
SEED              = sys.argv[2] if len(sys.argv) > 2 else None
VERTEX_COUNT      = sys.argv[3] if len(sys.argv) > 3 else None
TREE_TYPE         = sys.argv[4] if len(sys.argv) > 4 else None
WEIGHT_TYPE       = sys.argv[5] if len(sys.argv) > 5 else None
MAX_WEIGHT        = sys.argv[6] if len(sys.argv) > 6 else None
WEIGHT_PERCENT    = sys.argv[7] if len(sys.argv) > 7 else None
DEPTH_COEFF       = sys.argv[8] if len(sys.argv) > 8 else None


main({
    'TEST_INDEX':     TEST_INDEX,
    'SEED':           SEED,
    'VERTEX_COUNT':   VERTEX_COUNT,
    'TREE_TYPE':      TREE_TYPE,
    'WEIGHT_TYPE':    WEIGHT_TYPE,
    'MAX_WEIGHT':     MAX_WEIGHT,
    'WEIGHT_PERCENT': WEIGHT_PERCENT,
    'DEPTH_COEFF':    DEPTH_COEFF,
    })
