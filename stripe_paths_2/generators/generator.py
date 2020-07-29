#!/usr/bin/env python3
import os
import sys
import random
from tree import *
from tree_generators import *
from query_generators import *

sys.setrecursionlimit(10 ** 6)

def create_tree_generator(generator_type='random'):
    if generator_type == 'random':
        return RandomTreeGenerator()
    if generator_type == 'line':
        return LineTreeGenerator()
    if generator_type == 'depth':
        depth_coeff = float(os.getenv('DEPTH_COEFF', '1'))
        return DepthTreeGenerator(depth_coeff)

def create_query_generator(query_type='random'):
    if query_type == 'random':
        query_percent = float(os.getenv('QUERY_PERCENT', '0.1'))
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

def main():
    test_count = int(os.getenv('TEST_COUNT', '1'))
    test_start_index = int(os.getenv('TEST_START_INDEX', '0')) + 1
    test_dir = os.getenv('TEST_DIR')

    seed_value = os.getenv('SEED')
    vertex_count = int(os.getenv('VERTEX_COUNT', '1'))
    queries_count = int(os.getenv('QUERIES_COUNT', '1'))
    tree_type = os.getenv('TREE_TYPE', 'random')
    query_type = os.getenv('QUERY_TYPE', 'random')

    random.seed(f"{seed_value}_{test_start_index}")

    for test_index in range(test_start_index, test_start_index + test_count):
        tree_generator = create_tree_generator(tree_type)
        query_generator = create_query_generator(query_type)
        with open(os.path.join(test_dir, f"input{test_index}.txt"), 'w') as file:
            vertex_count_delta = min(vertex_count - 1, max(vertex_count // 10, 5))

            tree = tree_generator.generate(vertex_count + random.randint(-vertex_count_delta, 0))
            print_tree(tree, file=file)

            queries = query_generator.generate_queries(tree, random.randint(queries_count - queries_count // 10, queries_count))
            print_queries(list(queries), file=file)

        print(f"[TEST #{test_index: >3}] done")

if __name__ == '__main__':
    main()
