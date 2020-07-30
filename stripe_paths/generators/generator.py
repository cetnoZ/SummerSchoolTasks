#!/usr/bin/env python3
import os
import random
from tree import *
from tree_generators import *
from tree_labelers import *


def create_tree_generator(generator_type='random'):
    if generator_type == 'random':
        return RandomTreeGenerator()
    if generator_type == 'line':
        return LineTreeGenerator()
    elif generator_type == 'depth':
        depth_coeff = float(os.getenv('DEPTH_COEFF', '1'))
        return DepthTreeGenerator(depth_coeff)

def create_tree_labeler(labeler_type='random'):
    if labeler_type == 'random':
        percent = float(os.getenv('LABEL_PERCENT', '0.5'))
        return RandomTreeLabeler(percent)
    elif labeler_type == 'stripes':
        stripes_count = int(os.getenv('STRIPES_COUNT', '10'))
        k_type = os.getenv('K_TYPE', 'random')

        return StripesTreeLabeler(stripes_count, k_type)
    elif labeler_type == 'one_stripe':
        k_type = os.getenv('K_TYPE', 'random')

        return OneStripeTreeLabeler(k_type)

def main():
    test_count = int(os.getenv('TEST_COUNT', '1'))
    test_start_index = int(os.getenv('TEST_START_INDEX', '0')) + 1
    test_dir = os.getenv('TEST_DIR')

    seed_value = os.getenv('SEED')
    vertex_count = int(os.getenv('VERTEX_COUNT', '1'))
    tree_type = os.getenv('TREE_TYPE', 'random')
    label_type = os.getenv('LABEL_TYPE', 'random')

    random.seed(f"{seed_value}_{test_start_index}")

    for test_index in range(test_start_index, test_start_index + test_count):
        tree_generator = create_tree_generator(tree_type)
        tree_labeler = create_tree_labeler(label_type)
        with open(os.path.join(test_dir, f"input{test_index}.txt"), 'w') as file:
            vertex_count_delta = min(vertex_count - 1, max(vertex_count // 10, 5))
            tree = tree_generator.generate(vertex_count + random.randint(-vertex_count_delta, 0))
            tree_labeler.label_tree(tree)
            print_tree(tree, file=file)
        print(f"[TEST #{test_index: >3}] done")

if __name__ == '__main__':
    main()
