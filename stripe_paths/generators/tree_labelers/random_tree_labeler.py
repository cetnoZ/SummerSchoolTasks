import random
from .tree_labeler import *


class RandomTreeLabeler(TreeLabeler):
    def __init__(self, percent):
        TreeLabeler.__init__(self)
        self._percent = percent

    def label_tree(self, root):
        for child in root.children:
            self.label_node(child)

    def label_node(self, node):
        node.label = 1 if random.random() > self._percent else 0
        for child in node.children:
            self.label_node(child)
