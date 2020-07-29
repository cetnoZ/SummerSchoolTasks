#include <iostream>
#include <vector>
#include <set>
#include <unordered_set>
#include <algorithm>


using namespace std;

int n;
vector<int> lefts, rights, weights, parent;
vector<int> ltime, rtime;

void precalc(int v, int p = 0) {
    if (v == 0) {
        return;
    }
    static int counter = 0;
    ltime[v] = ++counter;

    precalc(lefts[v], v);
    precalc(rights[v], v);

    rtime[v] = ++counter;
}

bool inside(int higher, int lower) {
    return ltime[higher] <= ltime[lower] && rtime[lower] <= rtime[higher];
}

vector<int> get_path(int u, int v) {
    vector<int> path;
    while (!inside(u, v)) {
        path.push_back(u);
        u = parent[u];
    }

    while (v != u) {
        path.push_back(u);
        if (lefts[u] != 0 && inside(lefts[u], v)) {
            u = lefts[u];
        } else if (rights[u] != 0 && inside(rights[u], v)) {
            u = rights[u];
        }
    }
    path.push_back(v);
    return path;
}

void rotate_path(const vector<int>& path, int direction) {
    if (direction == +1) {
        for (int i = int(path.size()) - 2; i >= 0; i--) {
            swap(weights[path[i]], weights[path[i + 1]]);
        }
    } else if (direction == -1) {
        for (int i = 0; i + 1 < path.size(); i++) {
            swap(weights[path[i]], weights[path[i + 1]]);
        }
    }
}

long long calc_subanswer(const vector<int>& path) {
    long long sub_answer = 0;
    for (int v : path) {
        sub_answer += weights[v];
    }
    return sub_answer;
}

long long query(int u, int v) {
    vector<int> path = get_path(u, v);
    unordered_set<int> path_vertices(begin(path), end(path));

    long long answer = calc_subanswer(path);
    for (int vv = 1; vv <= n; vv++) {
        for (int uu = 1; uu <= n; uu++) {
            if (vv == uu) {
                continue;
            }
            vector<int> other_path = get_path(uu, vv);

            bool ok = (path_vertices.count(vv) == 0) && (path_vertices.count(uu) == 0);
            if (!ok) {
                continue;
            }

            rotate_path(other_path, +1);

            long long sub_answer = calc_subanswer(path);
            answer = min(answer, sub_answer);

            rotate_path(other_path, -1);
        }
    }
    return answer;
}

int main() {
    ios::sync_with_stdio(false);

    cin >> n;

    lefts.resize(n + 1);
    rights.resize(n + 1);
    weights.resize(n + 1);
    parent.resize(n + 1);

    for (int i = 1; i <= n; i++) {
        int l, r, w;
        cin >> l >> r >> w;
        
        lefts[i] = l;
        parent[l] = i;

        rights[i] = r;
        parent[r] = i;

        weights[i] = w;
    }

    ltime.resize(n + 1);
    rtime.resize(n + 1);
    precalc(1);

    int q;
    cin >> q;
    for (int i = 0; i < q; i++) {
        int u, v;
        cin >> u >> v;

        cout << query(u, v) << endl;
    }

    return 0;
}
