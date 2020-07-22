import random
from collections import deque
from .tree_labeler import *


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
