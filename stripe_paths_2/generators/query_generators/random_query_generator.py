import random
from tree import *
from .query_generator import *


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
