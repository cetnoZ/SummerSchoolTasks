#include <iostream>
#include <algorithm>
#include <vector>


using namespace std;

vector<vector<int>> g;
vector<int> used;
vector<int> cycles;
vector<int> stack;

void mark_all_cycles(int v, int p = -1) {
    if (used[v]) {
        for (auto it = stack.rbegin(); it != stack.rend() && *it != v; it++) {
            cycles[*it]++;
        }
        cycles[v]++;
        return;
    }
    stack.push_back(v);
    used[v] = true;

    for (int u : g[v]) {
        if (v == p) {
            continue;
        }
        mark_all_cycles(u, v);
    }

    used[v] = false;
    stack.pop_back();
}

bool query(int a, int b) {
    g[a].push_back(b);
    g[b].push_back(a);

    fill(begin(cycles), end(cycles), 0);

    mark_all_cycles(a);

    g[a].pop_back();
    g[b].pop_back();
    return all_of(begin(cycles), end(cycles), [] (int cycles_count) { return cycles_count <= 2; });
}

int main() {
    ios::sync_with_stdio(false);

    int n, m;
    cin >> n >> m;

    for (int i = 1; i <= m; i++) {
        int v, u;
        cin >> v >> u;

        v--;
        u--;

        g[v].push_back(u);
        g[u].push_back(v);
    }

    stack.reserve(n);

    int q;
    cin >> q;

    for (int i = 0; i < q; i++) {
        int a, b;
        cin >> a >> b;

        a--;
        b--;

        bool answer = query(a, b);
        cout << (answer ? "YES" : "NO") << endl;
     }

}
