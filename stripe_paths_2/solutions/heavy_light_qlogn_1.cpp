#include <iostream>
#include <algorithm>
#include <vector>
#include <set>


using namespace std;
const int N = 60e4;

struct range_t {
    int color;
    int cnt;
    int iterations;
};

struct seq_t {
    int size;
    bool sub_stripe_path;
    vector<range_t> ranges;

    static seq_t reverse(const seq_t& a) {
        seq_t result;
        result.size = a.size;
        result.sub_stripe_path = a.sub_stripe_path;
        result.ranges = a.ranges;

        ::reverse(begin(result.ranges), end(result.ranges));

        for (auto& range : result.ranges) {
            if (range.iterations > 1) {
                range.color = ((range.iterations - 1) & 1) ^ range.color;
            }
        }
        return result;
    }

    static seq_t compose(const seq_t& a, const seq_t& b) {
        if (!a.sub_stripe_path || !b.sub_stripe_path) {
            return {-1, false, {}};
        }

        seq_t result;
        result.sub_stripe_path = true;
        result.size = a.size + b.size;
        result.ranges = a.ranges;

        for (const auto& e : b.ranges) {
            append(result.ranges, e);
        }

        if (result.ranges.size() >= 4) {
            return {-1, false, {}};
        }
        return result;

    }

    static pair<seq_t, seq_t> split(const seq_t& seq, int i) {
        seq_t first;
        seq_t second;

        int index = 0;
        for (auto range : seq.ranges) {
            int l = index;
            int r = index + range.cnt * range.iterations - 1;
            if (r <= i) {
                seq_t::append(first.ranges, range);
                first.size += r - l + 1;
            } else if (i <= l) {
                seq_t::append(second.ranges, range);
                second.size += r - l + 1;
            } else {
                int id = i - l;

                int left_iterations = id / range.cnt;
                int left_cnt = id % range.cnt;

                int right_iterations = (r - l + 1 - id) / range.cnt;
                int right_cnt = (r - l + 1 - id) % range.cnt;

                int color = ((left_iterations - 1) % 2) ^ range.color;

                if (left_iterations > 0) {
                    seq_t::append(first.ranges, range_t {range.color, range.cnt, left_iterations});
                }
                if (left_cnt > 0) {
                    seq_t::append(first.ranges, range_t {color, left_cnt, 1});
                }
                if (right_cnt > 0) {
                    seq_t::append(second.ranges, range_t {color, right_cnt, 1});
                }
                if (right_iterations > 0) {
                    seq_t::append(second.ranges, range_t {color ^ 1, range.cnt, right_iterations});
                }

            }
            index = r + 1;
        }
        return make_pair(first, second);
    }


    static void append(vector<range_t>& ranges, const range_t& a) {
        range_t range = ranges.back();

        if (range.iterations == 1 && a.iterations == 1) {
            if (range.color == a.color) {
                ranges.back() = range_t {a.color, a.cnt + range.cnt, 1};
            } else {
                ranges.push_back(a);
            }
        } else if (range.iterations == 1 && a.iterations > 1) {
            if (range.color == a.color) {
                ranges.back().cnt += a.cnt;
                ranges.push_back(range_t {1 - a.color, a.cnt, a.iterations - 1});
            } else if (range.cnt == a.cnt) {
                ranges.back() = range_t {range.color, range.cnt, a.iterations + 1};
            } else {
                ranges.push_back(a);
            }
        } else if (range.iterations > 1 && a.iterations == 1) {
            int last_color = ((range.iterations - 1) & 1) ^ range.color;
            if (last_color == a.color) {
                ranges.back().iterations -= 1;
                ranges.push_back(range_t {a.color, a.cnt + ranges.back().cnt, 1});
            } else if (range.cnt == a.cnt) {
                ranges.back().iterations += 1;
            } else {
                ranges.push_back(a);
            }
        } else if (range.iterations > 1 && a.iterations > 1) {
            int last_color = ((range.iterations - 1) & 1) ^ range.color;
            if (last_color == a.color) {
                ranges.back().iterations -= 1;
                ranges.push_back(range_t {last_color, ranges.back().cnt, 1});
                append(ranges, a);
            } else if (range.cnt == a.cnt) {
                ranges.back().iterations += a.iterations;
            } else {
                ranges.push_back(a);
            }
        }
    }
};

struct seg_tree_t {
    struct node_t {
        seq_t seq;
        int l, r;

        node_t() 
            : seq(), l(-1), r(-1) {
        }
    };

    vector<node_t> nodes;
    int N;
    int cnt;
    int root;

    seg_tree_t(int N) {
        this->N = N;
        nodes.resize(4 * N);
        cnt = 0;
        
        root = create_node();
        vector<range_t> zeros = { range_t {0, N, 1} };
        nodes[root].seq = seq_t { N, true, zeros };
    }

    void modify(int ll, int rr, seq_t seq) {
        modify(ll, rr, 1, N, root, seq);
    }

    void modify(int ll, int rr, int l, int r, int v, seq_t seq) {
        expand(v, l, r);
        if (r < ll || rr < l) {
            return;
        }
        if (ll <= l && r <= rr) {
            nodes[v].seq = seq;
            return;
        }
        int c = (l + r) / 2;
        auto seqs = seq_t::split(seq, c - l + 1);
        modify(ll, rr, l, c, nodes[v].l, seqs.first);
        modify(ll, rr, c + 1, r, nodes[v].r, seqs.second);

        nodes[v].seq = seq_t::compose(nodes[nodes[v].l].seq, nodes[nodes[v].r].seq);
    }

    seq_t get(int ll, int rr) {
        return get(ll, rr, 1, N, root);
    }

    seq_t get(int ll, int rr, int l, int r, int v) {
        expand(v, l, r);
        if (r < ll || rr < l) {
            return seq_t { 0, true, {} };
        }
        if (ll <= l && r <= rr) {
            return nodes[v].seq;
        }
    
        int c = (l + r) / 2;
        seq_t left = get(ll, rr, l, c, nodes[v].l);
        seq_t right = get(ll, rr, c + 1, r, nodes[v].r);
        return seq_t::compose(left, right);
    }


    int create_node() {
        return cnt++;
    }

    void expand(int v, int l, int r) {
        if (nodes[v].l == -1 || nodes[v].r == -1) {
            int c = (l + r) / 2;
            auto seqs = seq_t::split(nodes[v].seq, c - l + 1);

            nodes[v].l = create_node();
            nodes[nodes[v].l].seq = seqs.first;

            nodes[v].r = create_node();
            nodes[nodes[v].r].seq = seqs.second;
        }
    }
};


vector<int> lefts, rights;
vector<int> parents, sizes;
vector<int> ltime, rtime;
vector<int> next_path;
vector<int> depths;
seg_tree_t tree(N);


void precalc(int v, int p = 0) {
    sizes[v] = 1;
    depths[v] = depths[p] + 1;

    for (int u : { lefts[v], rights[v] }) {
        if (u != 0 && u != p) {
            precalc(u, v);
            sizes[v] += sizes[u] + 1;
        }
    }

    if (lefts[v] != 0 && rights[v] != 0) {
        if (sizes[lefts[v]] < sizes[rights[v]]) {
            swap(lefts[v], rights[v]);
        }
    }
}

bool inside(int higher, int lower) {
    return ltime[higher] <= ltime[lower] && rtime[lower] <= rtime[higher];
}

void build_hld(int v, int p = 0) {
    static int time = 0;
    ltime[v] = ++time;

    for (int u : { lefts[v], rights[v] }) {
        if (u != 0 && u != p) {
            next_path[u] = ((u == lefts[v]) ? next_path[v] : u);
            build_hld(u, v);
        }
    }

    rtime[v] = time;
}

vector<pair<int, int>> get_up_segments(int& u, int& v) {
    vector<pair<int, int>> segments;
    while (!inside(next_path[u], v)) {
        segments.push_back(make_pair(next_path[u], u));
        u = parents[next_path[u]];
    }
    return segments;
}

vector<pair<int, int>> get_down_segments(int& u, int& v) {
    vector<pair<int, int>> segments;
    while (!inside(next_path[u], v)) {
        segments.push_back(make_pair(u, next_path[u]));
        u = parents[next_path[u]];
    }
    reverse(begin(segments), end(segments));
    return segments;
}

int lca(int u, int v) {
    get_up_segments(u, v);
    get_up_segments(v, u);
    
    if (inside(u, v)) {
        return u;
    } else {
        return v;
    }
}

vector<pair<int, int>> get_segments(int u, int v) {
    vector<pair<int, int>> segments;

    auto up_segments = get_up_segments(u, v);
    auto down_segments = get_down_segments(v, u);

    segments.insert(begin(segments), begin(up_segments), end(up_segments));
    if (u != v) {
        if (inside(u, v)) {
            int next_u;
            if (lefts[u] != 0 && inside(lefts[u], v)) {
                next_u = lefts[u];
            } else {
                next_u = rights[u];
            }
            segments.push_back(make_pair(next_u, v));
        } else {
            int prev_v;
            if (lefts[v] != 0 && inside(lefts[v], u)) {
                prev_v = lefts[v];
            } else {
                prev_v = rights[v];
            }
            segments.push_back(make_pair(u, prev_v));
        }
    }
    segments.insert(begin(segments), begin(down_segments), end(down_segments));
    return segments;
}

bool query(int u, int v) {
    vector<pair<int, int> > segments = get_segments(u, v);

    seq_t answer = {0, true, {}};
    for (const auto& segment : segments) {
        auto seq = tree.get(ltime[segment.first], ltime[segment.second]);
        answer = seq_t::compose(answer, seq);
    }
    return answer.sub_stripe_path && answer.ranges.size() == 1;
}

void colorize_path(int u, int v, int k) {
    vector<pair<int, int> > segments = get_segments(u, v);

    int lca_uv = lca(u, v);
    int path_length = depths[v] + depths[u] - 2 * depths[lca_uv];

    vector<range_t> ranges = {range_t {1, k, path_length / k}};
    seq_t seq = {path_length, true, ranges};

    for (const auto& segment : segments) {
        int length = abs(ltime[segment.first] - ltime[segment.second]) + 1;
        auto seqs = seq_t::split(seq, length);
        if (ltime[segment.first] < ltime[segment.second]) {
            tree.modify(ltime[segment.first], ltime[segment.second], seqs.first);
        } else {
            seqs.first = seq_t::reverse(seqs.first);
            tree.modify(ltime[segment.second], ltime[segment.first], seqs.first);
        }
    }
}


int main() {
    ios::sync_with_stdio(false);

    int n;
    cin >> n;

    lefts.resize(n + 1);
    rights.resize(n + 1);
    parents.resize(n + 1);

    for (int i = 0; i < n; i++) {
        int l, r;
        cin >> l >> r;

        l--;
        r--;

        if (l != -1) {
            parents[l] = i;
            lefts[i] = l;
        }

        if (r != -1) {
            parents[r] = i;
            rights[i] = r;
        }
    }

    ltime.resize(n + 1);
    rtime.resize(n + 1);
    sizes.resize(n + 1);
    depths.resize(n + 1);
    next_path.resize(n + 1);

    precalc(0);

    int q;
    cin >> q;

    for (int i = 0; i < n; i++) {
        int type;
        cin >> type;
        if (type == 1) {
            int u, v;
            cin >> u >> v;

            cout << query(u, v) << endl;
        } else {
            int u, v, k;
            cin >> u >> v >> k;

            colorize_path(u, v, k);
        }
    }

    return 0;
}

