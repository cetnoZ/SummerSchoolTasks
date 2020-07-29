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
    test_count = int(os.getenv('TEST_COUNT', '1'))
    test_start_index = int(os.getenv('TEST_START_INDEX', '0')) + 1
    test_dir = os.getenv('TEST_DIR')

    seed_value = os.getenv('SEED')
    tree_type = os.getenv('TREE_TYPE', 'random')
    average_cycle = int(os.getenv('AVERAGE_CYCLE', '10'))
    cycle_variation = float(os.getenv('CYCLE_VARIATION', '0.1'))
    vertex_count = int(os.getenv('VERTEX_COUNT', '1'))
    queries_count = int(os.getenv('QUERIES_COUNT', '1'))

    random.seed(f"{seed_value}_{test_start_index}")

    for test_index in range(test_start_index, test_start_index + test_count):
        tree_generator = create_tree_generator(tree_type)
        tree_expander = create_tree_expander(average_cycle, cycle_variation)

        current_vertex_count = random.randint(max(3, vertex_count - max(5, vertex_count // 10)), vertex_count)

        tree_vertex_count = random.randint(max(1, current_vertex_count // average_cycle), max(1, current_vertex_count // 2))

        with open(os.path.join(test_dir, f"input{test_index}.txt"), 'w') as file:
            tree = tree_generator.generate(tree_vertex_count)
            result_graph = tree_expander.expand_tree(tree, current_vertex_count)

            print_graph(result_graph, file=file)
            current_queries_count = random.randint(0, queries_count)
            queries = list(generate_queries(result_graph, current_queries_count))
            print_queries(queries, file=file)
        print(f"[TEST #{test_index: >3}] done")

if __name__ == '__main__':
    main()
