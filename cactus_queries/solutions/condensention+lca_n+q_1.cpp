#include <iostream>
#include <vector>
#include <set>

using namespace std;
const int K = 20;

vector<vector<int>> g;
vector<set<int>> gc;
vector<vector<int>> cycles;
vector<int> stack;
vector<int> used;
vector<int> cycle;

vector<vector<int>> parents_k;
vector<int> ltime, rtime;
vector<int> dp;

void find_cycles(int v, int p = 0) {
    if (cycle[v] != 0 || used[v] == 2) {
        return;
    } else if (used[v] == 1) {
        int cycle_id = cycles.size();
        cycles.resize(cycles.size() + 1);
        vector<int> &current_cycle = cycles.back();

        for (auto it = stack.rbegin(); it != stack.rend(); it++) {
            current_cycle.push_back(*it);
            cycle[*it] = cycle_id;
            if (*it == v) {
                break;
            }
        }
        return;
    }

    stack.push_back(v);
    used[v] = 1;

    for (int u : g[v]) {
        if (u == p) {
            continue;
        }
        find_cycles(u, v);
    }
    used[v] = 2;
    stack.pop_back();

;
    if (cycle[v] == 0) {
        cycle[v] = cycles.size();
        vector<int> current_cycle = { v };
        cycles.push_back(current_cycle);
    }
}

void precalc_lca(int v, int p = 0) {
    static int time = 0;

    dp[v] = dp[p] + (cycles[v].size() > 1);

    ltime[v] = ++time;
    parents_k[0][v] = p;
    for (int j = 1; j < K; j++) {
        parents_k[j][v] = parents_k[j - 1][parents_k[j - 1][v]];
    }

    for (int u : gc[v]) {
        if (u != p) {
            precalc_lca(u, v);
        }
    }
    rtime[v] = ++time;
}


bool inside(int higher, int lower) {
    return ltime[higher] <= ltime[lower] && rtime[lower] <= rtime[higher];
}

int lca(int v, int u) {
    if (inside(v, u)) {
        return v;
    }
    if (inside(u, v)) {
        return u;
    }

    for (int j = K - 1; j >= 0; j--) {
        int parent = parents_k[j][v];
        if (parent == 0 || inside(parent, u)) {
            continue;
        }
        v = parent;
    }
    return parents_k[0][v];
}


int main() {
    ios::sync_with_stdio(false);

    int n, m;
    cin >> n >> m;

    g.resize(n + 1);

    for (int i = 0; i < m; i++) {
        int v, u;
        cin >> v >> u;

        g[v].push_back(u);
        g[u].push_back(v);
    }

    used.resize(n + 1, 0);
    cycle.resize(n + 1, 0);
    cycles.push_back(vector<int>());
    find_cycles(1);

    gc.resize(n + 1);

    for (int v = 0; v < n; v++) {
        for (int u : g[v]) {
            int cv = cycle[v];
            int cu = cycle[u];
            if (cv != cu) {
                gc[cv].insert(cu);
                gc[cu].insert(cv);
            }
        }
    }

    dp.resize(n + 1);
    parents_k.resize(K, vector<int>(n + 1));
    ltime.resize(n + 1);
    rtime.resize(n + 1);

    precalc_lca(1);

    int q;
    cin >> q;
    for (int i = 0; i < q; i++) {
        int a, b;
        cin >> a >> b;
        
        int c_a = cycle[a], c_b = cycle[b];
        int c_lca = lca(c_a, c_b);
        int c_pred_lca = parents_k[0][c_lca];
        int answer = dp[c_a] + dp[c_b] - dp[c_pred_lca] - dp[c_lca];

        cout << (answer == 0 ? "Yes" : "No") << "\n";
    }

    return 0;
}
