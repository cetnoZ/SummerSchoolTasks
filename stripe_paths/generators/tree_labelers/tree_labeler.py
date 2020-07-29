from collections import deque
from tree import *


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
