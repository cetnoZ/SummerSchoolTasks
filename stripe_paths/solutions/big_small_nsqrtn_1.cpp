#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>
#include <tuple>


using namespace std;
const int N = 2e5 + 100;
const int K = 20;
const int ROOT = 1;
int K_THRESHOLD = llround(sqrt(N));


vector<vector<pair<int, int>>> g;
vector<int> colors;
vector<int> depths;
vector<int> parents;
vector<int> up_next;
vector<int> up_count;
vector<int> up_dp;

void precalc(int v, int p = 0) {
    up_next[v] = (colors[v] == colors[p]) ? up_next[p] : p;
    if (colors[v] == colors[p]) {
        up_next[v] = up_next[p];
    } else {
        up_next[v] = p;
    }

    for (const auto& edge : g[v]) {
        int u = edge.first;
        int u_color = edge.second;
        if (u != p) {
            colors[u] = u_color;
            depths[u] = depths[v] + 1;
            parents[u] = v;
            precalc(u, v);
        }
    }
}


long long calc_up_cnt(int v, int color) {
    int v_color = colors[v];
    int u = up_next[v];

    return (v_color == color) ? (depths[v] - depths[u]) : 0;
}

tuple<int, int, int> calc_segment_up(int v, int k) {
    int up_cnt_0 = calc_up_cnt(v, 0);
    int up_v_0 = up_next[v];
    v = up_v_0;

    int up_cnt_1 = calc_up_cnt(v, 1);
    int up_v_1 = up_next[v];
    v = up_v_1;

    return {v, up_cnt_0, up_cnt_1};
}

long long calc_up(int v, int k) {
    long long answer = 0;
    while (v > 0) {
        int up_cnt_0, up_cnt_1;
        tie(v, up_cnt_0, up_cnt_1) = calc_segment_up(v, k);
        if (up_cnt_0 == k && up_cnt_1 >= k) {
            answer += 1;
        }
        if (up_cnt_0 != k || up_cnt_1 != k) {
            break;
        }
    }
    return answer;
}

long long calc_big_k_answers(int n, int min_k) {
    long long answer = 0;
    for (int v = 1; v <= n; v++) {
        int k = calc_up_cnt(v, 0);
        if (k >= max(min_k, 1)) {
            long long up_cnt = calc_up(v, k);
            answer += up_cnt;
        }
    }
    return answer;
}

long long calc_dp(int v, int k, int p = -1) {
    long long answer = 0;
    
    int u, up_cnt_0, up_cnt_1;
    tie(u, up_cnt_0, up_cnt_1) = calc_segment_up(v, k);
    if (up_cnt_0 == k && up_cnt_1 == k) {
        up_dp[v] = up_dp[u] + 1;
    } else if (up_cnt_0 == k && up_cnt_1 > k) {
        up_dp[v] = 1;
    } else {
        up_dp[v] = 0;
    }

    answer += up_dp[v];

    for (const auto& edge : g[v]) {
        int u = edge.first;
        if (u != p) {
            answer += calc_dp(u, k, v);
        }
    }
    return answer;
}

long long calc_small_k_answers(int n, int max_k) {
    long long answer = 0;
    for (int k = 1; k < max_k; k++) {
        answer += calc_dp(ROOT, k);
    }
    return answer;
}

int main() {
    ios::sync_with_stdio(false);

    int n;
    cin >> n;

    g.resize(n + 1);
    colors.resize(n + 1);
    depths.resize(n + 1);
    parents.resize(n + 1);
    up_next.resize(n + 1);
    up_count.resize(n + 1);
    up_dp.resize(n + 1);

    for (int i = 1; i < n; i++) {
        int u, v, color;
        cin >> u >> v >> color;

        g[u].push_back(make_pair(v, color));
        g[v].push_back(make_pair(u, color));
    }

    precalc(ROOT);

    long long answer = 0;
    int k_threshold = K_THRESHOLD;
    answer += calc_big_k_answers(n, k_threshold);
    answer += calc_small_k_answers(n, k_threshold);

    cout << answer << "\n";

    return 0;
}
