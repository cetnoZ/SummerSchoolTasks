#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "../../testlib/testlib.h"


using namespace std;
const int ROOT = 1;
const long long MAX_WEIGHT = 1e9;
const int MAX_N = 500;
const int MAX_SWAPS = 3e5;

struct node_t {
    node_t *left, *right, *parent;
    long long weight;
    int index;
};

struct tree_t {
    node_t *root;
    vector<node_t *> nodes;
};

struct swap_t {
    int u, v;
};

int n;
vector<int> lefts;
vector<int> rights;
vector<long long> weights;


tree_t build_initial_tree() {
    vector<node_t *> nodes(1, nullptr);

    generate_n(back_inserter(nodes), n, [] () {
        return new node_t();
    });

    for (int v = 1; v <= n; v++) {
        node_t &node = *nodes[v];
        if (lefts[v] != 0) {
            node.left  = nodes[lefts[v]];
            node.left->parent = nodes[v];
        }
        if (rights[v] != 0) {
            node.right = nodes[rights[v]];
            node.right->parent = nodes[v];
        }
        node.weight = weights[v];
        node.index = v;
    }
    return { nodes[1], nodes };
}

node_t *rotate_nodes(node_t *v, node_t *u) {
    if (u->left == v || u->right == v) {
        swap(u, v);
    }

    node_t *parent = v->parent;
    if (parent != nullptr && parent->left == v) {
        parent->left = u;
    } else if (parent != nullptr && parent->right == v) {
        parent->right = u;
    }

    node_t *left_subtree, *middle_subtree, *right_subtree;
    if (v->left == u) {
        left_subtree = u->left;
        middle_subtree = u->right;
        right_subtree = v->right;

        u->right = v;
        u->left = left_subtree;
        v->left = middle_subtree;
        v->right = right_subtree;
    } else if (v->right == u) {
        left_subtree = v->left;
        middle_subtree = u->right;
        right_subtree = u->right;

        u->left = v;
        v->left = left_subtree;
        v->right = middle_subtree;
        u->right = right_subtree;
    }

    u->parent = parent;
    v->parent = u;
    return u;
}

void rotate_nodes(tree_t& tree, node_t *v, node_t *u) {
    if (v == nullptr || u == nullptr) {
        quitf(_fail, "rotation with null");
    }

    if (u->left != v && u->right != v && v->left != u && v->right != u) {
        quitf(_wa, "invalid rotation: v_index = %d, u_index = %d", v->index, u->index);
    }

    if (u->left == v || u->right == v) {
        swap(v, u);
    }

    if (v == tree.root) {
        tree.root = u;
    }
    rotate_nodes(v, u);
}

vector<swap_t> read_answer(InStream& stream) {
    vector<swap_t> swaps;

    int count = stream.readInt(1, MAX_SWAPS, "number of swaps");
    for (int i = 1; i <= count; i++) {
        int u = stream.readInt(1, n, format("first_rotation_vertex[%d]", i));
        int v = stream.readInt(1, n, format("second_rotation_vertex[%d]", i));
        if (u == v) {
            stream.quitf(_wa, "rotation with one vertex %d %d", v, u);
        }
        swaps.push_back({ u, v });
    }
    return swaps;
}

node_t *copy_node(tree_t& tree, node_t *node, node_t *parent = nullptr) {
    if (node == nullptr) {
        return nullptr;
    }
    node_t *new_node = new node_t(*node);
    new_node->left = copy_node(tree, node->left, new_node);
    new_node->right = copy_node(tree, node->right, new_node);
    new_node->parent = parent;
    tree.nodes[new_node->index] = new_node;
    return new_node;
}

tree_t copy_tree(tree_t tree) {
    tree_t new_tree(tree);
    new_tree.root = copy_node(new_tree, tree.root);
    return new_tree;
}

long long calc_answer(node_t *node, long long depth = 1) {
    if (node == nullptr) {
        return 0;
    }
    long long answer = depth * node->weight;
    answer += calc_answer(node->left);
    answer += calc_answer(node->right);
    return answer;
}

long long calc_answer(tree_t tree, const vector<swap_t>& swaps) {
    tree_t new_tree = copy_tree(tree);

    for (const swap_t& swap : swaps) {
        rotate_nodes(new_tree, new_tree.nodes[swap.u], new_tree.nodes[swap.v]);
    }
    return calc_answer(tree.root);
}

int main(int argc, char **argv) {
    registerTestlibCmd(argc, argv);

    n = inf.readInt(1, MAX_N, "number of vertices");

    lefts.resize(n + 1);
    rights.resize(n + 1);
    weights.resize(n + 1);

    for (int i = 1; i <= n; i++) {
        int left = inf.readInt(0, n, format("left[%d]", i).c_str());
        int right = inf.readInt(0, n, format("right[%d]", i).c_str());
        long long weight = inf.readLong(0, MAX_WEIGHT, format("weight[%d]", i).c_str());

        lefts[i] = left;
        rights[i] = right;
        weights[i] = weight;
    }

    tree_t tree = build_initial_tree();


    vector<swap_t> jury_swaps = read_answer(ans);
    long long jury_answer = calc_answer(tree, jury_swaps);

    vector<swap_t> participant_swaps = read_answer(ouf);
    long long participant_answer = calc_answer(tree, participant_swaps);

    if (jury_answer < participant_answer) {
        quitf(_wa, "jury has the better answer: jury_answer = %lld, participant_answer = %lld\n", jury_answer, participant_answer);
    } else if (jury_answer == participant_answer) {
        quitf(_ok, "answer = %lld\n", participant_answer);
    } else {
        quitf(_fail, ":( participant has the better answer: jury_answer = %lld, participant_answer = %lld\n", jury_answer, participant_answer);
    }
}
