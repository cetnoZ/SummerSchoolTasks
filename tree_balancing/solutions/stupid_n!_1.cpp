#include <iostream>
#include <vector>
#include <utility>
#include <limits>
#include <algorithm>
#include <iterator>


using namespace std;
const int N = 2e5 + 100;
const int ROOT = 1;

struct node_t {
    node_t *left, *right, *parent;
    long long weight;
    int index;
};

struct swap_t {
    int v, u;
};

vector<int> lefts;
vector<int> rights;
vector<long long> weights;

vector<int> vertices(1);

vector<vector<int>> splits;

void precalc(int v) {
    if (lefts[v] != 0) {
        precalc(lefts[v]);
    }
    
    vertices.push_back(v);
     
    if (rights[v] != 0) {
        precalc(rights[v]);
    }
}

void init_splits(int l, int r) {
    if (l <= r) {
        splits[l][r] = l;
        init_splits(l + 1, r);
    }
}

bool next_split(int l, int r) {
    if (l >= r) {
        return true;
    }
    if (splits[l][r] == r) {
        return next_split(l, r - 1);
    } else {
        if (!next_split(splits[l][r] + 1, r)) {
            return false;
        }
        if (!next_split(l, splits[l][r] - 1)) {
            init_splits(splits[l][r] + 1, r);
            return false;
        }
        splits[l][r]++;
        init_splits(splits[l][r] + 1, r);
        init_splits(l, splits[l][r] - 1);
        return false;
    }
}

void print_tree(int l, int r, const string& prefix = "", bool is_left = false) {

    clog << prefix << (is_left ? "├─" : "└─");

    if (l > r) {
        clog << endl;
    } else {
        int vertex = vertices[splits[l][r]];
        clog << vertex << " (" << weights[vertex] << ")" << endl;
    }

    if (l < r) {
        string new_prefix = prefix + (is_left ? "│ " : "  ");
        print_tree(l, splits[l][r] - 1, new_prefix, true);
        print_tree(splits[l][r] + 1, r, new_prefix, false);
    }
}

long long calc_cost(int l, int r, long long height = 1) {
    if (l > r) {
        return 0;
    }

    int vertex = vertices[splits[l][r]];
    long long cost = height * weights[vertex];

    cost += calc_cost(l, splits[l][r] - 1, height + 1);
    cost += calc_cost(splits[l][r] + 1, r, height + 1);

    return cost;
}

node_t *build_result_tree(const vector<vector<int>>& splits, int l, int r, node_t *parent = nullptr) {
    if (l > r) {
        return nullptr;
    }
    int vertex = vertices[splits[l][r]];

    node_t *node = new node_t();
    node->index = vertex;
    node->parent = parent;
    node->left = build_result_tree(splits, l, splits[l][r] - 1, node);
    node->right = build_result_tree(splits, splits[l][r] + 1, r, node);
    node->weight = weights[vertex];

    return node;
}

node_t *build_initial_tree(int n) {
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
            node.left->parent = nodes[v];
        }
        node.weight = weights[v];
        node.index = v;
    }
    return nodes[ROOT];
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

void align_tree(node_t *node, vector<swap_t>& steps) {
    while (node != nullptr) {
        while (node->left != nullptr) {
            steps.push_back({node->index, node->left->index});
            node = rotate_nodes(node, node->left);
        }

        node = node->right;
    }
}

int main() {
    ios::sync_with_stdio(false);

    int n;
    cin >> n;

    lefts.resize(n + 1);
    rights.resize(n + 1);
    weights.resize(n + 1);

    splits.resize(n + 1, vector<int>(n + 1));

    for (int v = 1; v <= n; v++) {
        int left, right, weight;
        cin >> left >> right >> weight;

        lefts[v] = left;
        rights[v] = right;
        weights[v] = weight;
    }

    node_t *initial_tree_root = build_initial_tree(n);

    precalc(ROOT);
    init_splits(1, n);

    long long answer = numeric_limits<long long>::max();
    auto splits_answer = splits;

    do {
        long long sub_answer = calc_cost(1, n);
        if (sub_answer < answer) {
            answer = sub_answer;
            splits_answer = splits;
        }
    } while (!next_split(1, n));

    node_t *result_tree_root = build_result_tree(splits_answer, 1, n);

    vector<swap_t> initial_tree_steps, result_tree_steps;

    align_tree(initial_tree_root, initial_tree_steps);
    align_tree(result_tree_root, result_tree_steps);

    reverse(begin(result_tree_steps), end(result_tree_steps));

    vector<swap_t> steps = move(initial_tree_steps);
    steps.insert(end(steps), make_move_iterator(begin(result_tree_steps)), make_move_iterator(end(result_tree_steps)));

    cout << steps.size() << "\n";

    for (auto& step : steps) {
        cout << step.u << " " << step.v << "\n";
    }

    return 0;
}
