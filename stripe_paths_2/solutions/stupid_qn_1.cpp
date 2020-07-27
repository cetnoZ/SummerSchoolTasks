#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <unordered_set>
#include <map>


using namespace std;

vector<int> lefts, rights, left_colors, right_colors;

vector<int> parents, parent_colors;

vector<int> ltime, rtime;

void precalc(int v, int p = -1) {
    static int time = 0;
    ltime[v] = ++time;

    for (int u : {lefts[v], rights[v]}) {
        if (u != p) {
            precalc(u, v);
        }
    }

    rtime[v] = ++time;
}

bool inside(int higher, int lower) {
    return ltime[higher] <= ltime[lower] && rtime[lower] <= rtime[higher];
}

vector<int> get_path(int u, int v) {
    vector<int> path;
    while (!inside(u, v)) {
        path.push_back(u);
        u = parents[u];
    }
    while (u != v) {
        path.push_back(u);
        for (int e : {lefts[u], rights[u]}) {
            if (inside(e, v)) {
                u = e;
                break;
            }
        }
    }
    path.push_back(v);
    return path;
}

int get_k(const vector<int>& path) {
    int k = 0;
    int v = path[0];
    for (int i = 1; i < path.size(); i++) {
        int u = path[i];
        if (lefts[v] == u && left_colors[v] != 1) {
            break;
        } else if (rights[v] == u && right_colors[v] != 1) {
            break;
        } else if (parents[v] == u && parent_colors[v] != 1) {
            break;
        }
        v = u;
        k++;
    }
    return k;
}

bool query(int u, int v) {
    vector<int> path = get_path(u, v);
    int k = get_k(path);

    if (k == 0 || ((int(path.size()) - 1) % (2 * k)) != 0) {
        return false;
    }

    int prev_v = path[0];
    for (int i = 1; i < path.size(); i++) {
        int cur_v = path[i];
        int cur_c = 1 - ((i - 1) / k) % 2;
        if (lefts[prev_v] == cur_v && left_colors[prev_v] != cur_c) {
            return false;
        } else if (rights[prev_v] == cur_v && right_colors[prev_v] != cur_c) {
            return false;
        } else if (parents[prev_v] == cur_v && parent_colors[prev_v] != cur_c) {
            return false;
        }
    }
    return true;
}

void color_path(int u, int v, int k) {
    vector<int> path = get_path(u, v);

    if (k == 0 || ((int(path.size()) - 1) % (2 * k)) != 0) {
        exit(-100);
    }

    int prev_v = path[0];
    for (int i = 1; i < path.size(); i++) {
        int cur_v = path[i];
        int cur_c = 1 - ((i - 1) / k) % 2;
        if (lefts[prev_v] == cur_v) {
            left_colors[prev_v] = cur_c;
            parent_colors[cur_v] = cur_c;
        } else if (rights[prev_v] == cur_v) {
            left_colors[prev_v] = cur_c;
            parent_colors[cur_v] = cur_c;
        } else if (parents[prev_v] == cur_v) {
            parent_colors[prev_v] = cur_c;
            if (lefts[cur_v] == prev_v) {
                left_colors[cur_v] = cur_c;
            } else if (rights[cur_v] == prev_v) {
                right_colors[cur_v] = cur_c;
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);

    int n;
    cin >> n;


    lefts.resize(n + 1);
    left_colors.resize(n + 1);
    rights.resize(n + 1);
    right_colors.resize(n + 1);
    parents.resize(n + 1);
    parent_colors.resize(n + 1);

    for (int i = 0; i < n; i++) {
        int l, r, lc, rc;
        cin >> l >> lc >> r >> rc;

        l--;
        r--;

        if (l != -1) {
            parents[l] = i;
            parent_colors[l] = lc;
            lefts[i] = l;
            left_colors[i] = lc;
        }

        if (r != -1) {
            parents[r] = i;
            parent_colors[r] = rc;
            rights[i] = r;
            right_colors[i] = rc;
        }
    }

    ltime.resize(n + 1);
    rtime.resize(n + 1);

    precalc(0);

    int q;
    cin >> q;

    for (int i = 0; i < q; i++) {
        int type;
        cin >> type;
        if (type == 1) {
            int u, v;
            cin >> u >> v;

            cout << query(u, v) << endl;
        } else {
            int u, v, k;
            cin >> u >> v >> k;

            color_path(u, v, k);
        }
    }
    return 0;
}
