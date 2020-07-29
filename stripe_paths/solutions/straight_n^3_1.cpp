#include <iostream>
#include <vector>
#include <utility>


using namespace std;
const int N = 2e5 + 100;

vector<vector<pair<int, int>>> g;


long long calc_subtree_answer(int v, int k, int p, int depth = 0) {
    long long answer = 0;
    if (depth != 0 && depth % (2 * k) == 0) {
        answer += 1;
    }

    int current_color = 1 - (depth / k) % 2;

    for (const auto& edge : g[v]) {
        int u = edge.first;
        int color = edge.second;
        if (u != p && current_color == color) {
            answer += calc_subtree_answer(u, k, v, depth + 1);
        }
    }
    return answer;
}

long long calc_answer(int n, int v, int p = -1) {
    long long answer = 0;

    for (int k = 1; k <= n; k++) {
        answer += calc_subtree_answer(v, k, p);
    }

    for (const auto& edge : g[v]) {
        int u = edge.first;
        if (u != p) {
            answer += calc_answer(n, u, v);
        }
    }
    return answer;
}

int main() {
    ios::sync_with_stdio(false);

    int n;
    cin >> n;

    g.resize(n);

    for (int i = 1; i < n; i++) {
        int u, v, color;
        cin >> u >> v >> color;

        u--;
        v--;

        g[u].push_back(make_pair(v, color));
        g[v].push_back(make_pair(u, color));
    }

    long long answer = calc_answer(n, 0);

    cout << answer << "\n";

    return 0;
}
