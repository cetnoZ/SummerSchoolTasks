#!/usr/bin/env python3
import os
import random
from graph import *
from tree_generators import *
from tree_expanders import *


def create_tree_generator(generator_type='random'):
    if generator_type == 'random':
        return RandomTreeGenerator()
    if generator_type == 'line':
        return LineTreeGenerator()
    if generator_type == 'depth':
        depth_coeff = float(os.getenv('DEPTH_COEFF', '1'))
        return DepthTreeGenerator(depth_coeff)

def create_tree_expander(average_cycle, cycle_variation):
    return RandomTreeExpander(average_cycle, cycle_variation)

def main():
    test_count = int(os.getenv('TEST_COUNT', '1'))
    test_start_index = int(os.getenv('TEST_START_INDEX', '0')) + 1
    test_dir = os.getenv('TEST_DIR')

    seed_value = os.getenv('SEED')
    tree_type = os.getenv('TREE_TYPE', 'random')
    average_cycle = int(os.getenv('AVERAGE_CYCLE', '10'))
    cycle_variation = float(os.getenv('CYCLE_VARIATION', '0.1'))
    vertex_count = int(os.getenv('VERTEX_COUNT', '1'))

    random.seed(f"{seed_value}_{test_start_index}")

    for test_index in range(test_start_index, test_start_index + test_count):
        tree_generator = create_tree_generator(tree_type)
        tree_expander = create_tree_expander(average_cycle, cycle_variation)

        current_vertex_count = vertex_count - random.randint(0, min(vertex_count - 1, max(vertex_count // 10, 5)))

        tree_vertex_count = random.randint(max(1, current_vertex_count // average_cycle), max(1, current_vertex_count // 2))

        with open(os.path.join(test_dir, f"input{test_index}.txt"), 'w') as file:
            tree = tree_generator.generate(tree_vertex_count)
            result_graph = tree_expander.expand_tree(tree, current_vertex_count)

            print_graph(result_graph, file=file)
        print(f"[TEST #{test_index: >3}] done")

if __name__ == '__main__':
    main()
