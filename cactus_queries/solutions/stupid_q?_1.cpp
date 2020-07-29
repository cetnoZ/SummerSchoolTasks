#include <iostream>
#include <algorithm>
#include <vector>
#include <set>


using namespace std;

vector<vector<int>> g;
vector<int> used;
vector<int> stack;

int cycles_count = 0;

void find_all_cycles(int s, int v, int p = -1) {
    if (v == s && p != -1) {
        cycles_count += 1;
        return;
    } if (used[v] == 1) {
        return;
    }

    used[v] = 1;
    stack.push_back(v);

    for (int u : g[v]) {
        if (u != p) {
            find_all_cycles(s, u, v);
        }
    }

    used[v] = 0;
    stack.pop_back();
}

bool query(int n, int a, int b) {
    g[a].push_back(b);
    g[b].push_back(a);

    fill(begin(used), end(used), 0);

    bool answer = true;
    for (int i = 0; i < n; i++) {
        cycles_count = 0;
        find_all_cycles(i, i);
        answer &= cycles_count <= 2;
    }

    g[a].pop_back();
    g[b].pop_back();
    return answer;
}

int main() {
    ios::sync_with_stdio(false);

    int n, m;
    cin >> n >> m;

    g.resize(n + 1);

    for (int i = 1; i <= m; i++) {
        int v, u;
        cin >> v >> u;

        v--;
        u--;

        g[v].push_back(u);
        g[u].push_back(v);
    }

    used.resize(n);
    stack.reserve(n);

    int q;
    cin >> q;

    for (int i = 0; i < q; i++) {
        int a, b;
        cin >> a >> b;

        a--;
        b--;

        bool answer = query(n, a, b);
        cout << (answer ? "Yes" : "No") << endl;
     }

}
