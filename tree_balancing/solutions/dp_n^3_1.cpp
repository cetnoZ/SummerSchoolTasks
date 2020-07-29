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
vector<vector<long long>> weight_sums;
vector<vector<long long>> dp;

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

long long calc_answer(int ll, int rr) {
    for (int l = ll - 1; l <= rr + 1; l++) {
        for (int r = ll - 1; r < l; r++) {
            dp[l][r] = 0;
        }
    }
    for (int r = ll; r <= rr; r++) {
        for (int l = ll; l <= r; l++) {
            long long &value = dp[l][r];
            for (int c = l; c <= r; c++) {
                long long new_value = dp[l][c - 1] + weight_sums[l][r] + dp[c + 1][r];
                if (value > new_value) {
                    value = new_value;
                    splits[l][r] = c;
                }
            }
        }
    }
    return dp[ll][rr];
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

void print_tree(const node_t *node, const string& prefix = "", bool is_left = false) {
    clog << prefix << (is_left ? "├─" : "└─");

    if (node == nullptr) {
        clog << endl;
    } else {
        int vertex = node->index;
        clog << vertex << " (" << weights[vertex] << ")" << endl;
    }

    if (node->left != nullptr || node->right != nullptr) {
        string new_prefix = prefix + (is_left ? "│ " : "  ");
        print_tree(node->left, new_prefix, true);
        print_tree(node->right, new_prefix, false);
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
            node.right->parent = nodes[v];
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

    weight_sums.resize(n + 1, vector<long long>(n + 1, 0));
    dp.resize(n + 2, vector<long long>(n + 2, (long long)1e17));
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
    for (int l = 1; l <= n; l++) {
        for (int r = l; r <= n; r++) { 
            weight_sums[l][r] = weight_sums[l][r - 1] + weights[vertices[r]];
        }
    }

    long long answer = calc_answer(1, n);
    node_t *result_tree_root = build_result_tree(splits, 1, n);

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
