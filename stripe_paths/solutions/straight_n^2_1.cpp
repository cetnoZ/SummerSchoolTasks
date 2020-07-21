#include <iostream>
#include <vector>
#include <utility>


using namespace std;
const int N = 2e5 + 100;

vector<vector<pair<int, int>>> g;
vector<int> depths;
vector<int> colors;
vector<int> parents;

void precalc(int v, int depth = 0, int p = -1) {
    if (depth == 0) {
        colors[v] = -1;
    }

    depths[v] = depth;
    parents[v] = p;

    for (const auto& edge : g[v]) {
        int u = edge.first;
        int color = edge.second;
        if (u != p) {
             precalc(u, depth + 1, v);
             colors[u] = color;
        }
    }
}

long long calc_answer_up(int v, int k) {
    long long answer = 0;
    bool ok = true;

    while (ok) {
        int up_cnt_0 = 0;
        int up_cnt_1 = 1;

        for (int i = 0; i < k && v > 0 && colors[v] == 0; v = parents[v]) {
            ++up_cnt_0;
        }
        
        for (int i = 0; i < k && v > 0 && colors[v] == 1; v = parents[v]) {
            ++up_cnt_1;
        }
        ok = (up_cnt_0 == k) && (up_cnt_1 == k);
        answer += ok;
    }
    return answer;
}

int main() {
    ios::sync_with_stdio(false);

    int n;
    cin >> n;

    g.resize(n);
    parents.resize(n);
    depths.resize(n);
    colors.resize(n);

    for (int i = 1; i < n; i++) {
        int u, v, color;
        cin >> u >> v >> color;

        u--;
        v--;

        g[u].push_back(make_pair(v, color));
        g[v].push_back(make_pair(u, color));
    }

    precalc(0);

    long long answer = 0;
    for (int v = 0; v < n; v++) {
        int k = 0;

        for (int u = v; u > 0 && colors[u] == 0; u = parents[u]) {
            k++;
        }

        if (k > 0) {
            answer += calc_answer_up(v, k);
        }
    }

    cout << answer << "\n";

    return 0;
}
