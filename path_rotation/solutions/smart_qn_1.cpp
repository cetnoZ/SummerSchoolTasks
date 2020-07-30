#include <iostream>
#include <vector>
#include <unordered_set>
#include <limits>



using namespace std;

int n;
vector<int> lefts;
vector<int> rights;
vector<int> weights;
vector<int> parent;

vector<int> ltime;
vector<int> rtime;
vector<bool> used;

struct neighbor_t {
    long long in_value;
    long long out_value;
};

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

vector<neighbor_t> get_neighbors(int v) {
    vector<neighbor_t> neighbors;

    for (int u : { lefts[v], parent[v], rights[v] }) {
        if (u != 0 && !used[u]) {
            neighbor_t neighbor;
            neighbor.in_value = weights[u];
            neighbor.out_value = weights[v];
            neighbors.push_back(neighbor);
        }
    }
    return neighbors;
}

long long calc_path_weight(const vector<int>& path) {
    long long weight = 0;
    for (int v : path) {
        weight += weights[v];
    }
    return weight;
}

long long query(int u, int v) {
    auto path = get_path(u, v);
    vector<vector<neighbor_t>> neighbors(path.size());

    for (int e : path) {
        used[e] = true;
    }

    for (int i = 0; i < path.size(); i++) {
        int v = path[i];
        auto v_neighbors = get_neighbors(v);
        neighbors[i] = v_neighbors;
    }

    long long path_weight = calc_path_weight(path);
    long long answer = path_weight;
    long long sub_answer;

    long long max_out_value = numeric_limits<long long>::min();
    long long min_in_value = numeric_limits<long long>::max();

    bool was = false;
    for (int i = 0; i < path.size(); i++) {
       for (int neighbor_i = 0; neighbor_i < neighbors[i].size(); neighbor_i++) {
           const neighbor_t& first_neighbor = neighbors[i][neighbor_i];
           for (int neighbor_j = 0; neighbor_j < neighbors[i].size(); neighbor_j++) {
               const neighbor_t& second_neighbor = neighbors[i][neighbor_j];
               if (neighbor_i == neighbor_j) {
                   continue;
               }

               sub_answer = path_weight - first_neighbor.out_value + second_neighbor.in_value;
               answer = min(answer, sub_answer);
           }

           if (was) {
               sub_answer = path_weight - first_neighbor.out_value + min_in_value;
               answer = min(answer, sub_answer);

               sub_answer = path_weight - max_out_value + first_neighbor.in_value;
               answer = min(answer, sub_answer);
           }
       }

       for (int neighbor_i = 0; neighbor_i < neighbors[i].size(); neighbor_i++) {
           const neighbor_t& neighbor = neighbors[i][neighbor_i];
           min_in_value = min(min_in_value, neighbor.in_value);
           max_out_value = max(max_out_value, neighbor.out_value);
           was = true;
       }
    }

    for (int e : path) {
        used[e] = false;
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
    used.resize(n + 1);

    precalc(1);

    int q;
    cin >> q;
    for (int i = 0; i < q; i++) {
        int u, v;
        cin >> u >> v;

        long long answer = query(u, v);
        cout << answer << "\n";
    }
    return 0;
}

