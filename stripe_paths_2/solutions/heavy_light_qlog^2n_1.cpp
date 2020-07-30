#include <iostream>
#include <algorithm>
#include <vector>
#include <set>


#define clog if (0) clog

using namespace std;
const int N = 60e4;

struct range_t {
    int color;
    int cnt;
    int iterations;

    range_t() 
        : range_t(1, 0, 0) {
    }

    range_t(int color, int cnt, int iterations) 
        : color(color), cnt(cnt), iterations(iterations) {
    }

    int get_last_color() {
        return color ^ (iterations & 1) ^ 1;
    }
};

static ostream& operator<<(ostream& os, const range_t& range) {
    os << "( " << range.color << ", " << range.cnt << ", " << range.iterations << " )";
    return os;
}

struct seq_t;
ostream& operator<<(ostream& os, const seq_t& seq);

struct seq_t {
    int size;
    bool sub_stripe_path;
    vector<range_t> ranges;

    seq_t() 
        : seq_t(0, true, {}) {
    }

    seq_t(int size, bool sub_stripe_path, const std::vector<range_t>& ranges) 
        : size(size), sub_stripe_path(sub_stripe_path), ranges(ranges) {
    }

    static seq_t reverse(const seq_t& a) {
        seq_t result;
        result.size = a.size;
        result.sub_stripe_path = a.sub_stripe_path;
        result.ranges = a.ranges;

        ::reverse(begin(result.ranges), end(result.ranges));

        for (auto& range : result.ranges) {
            if (range.iterations > 1) {
                range.color = range.get_last_color();
            }
        }
        clog << "reverse " << a << " => " << result << endl;
        return result;
    }

    static seq_t compose(const seq_t& a, const seq_t& b) {
        if (!a.sub_stripe_path || !b.sub_stripe_path) {
            return {0, 0, {}};
        }
        seq_t result;
        result.size = a.size + b.size;
        result.sub_stripe_path = a.sub_stripe_path && b.sub_stripe_path;
        if (result.sub_stripe_path) {
            result.ranges.insert(end(result.ranges), begin(a.ranges), end(a.ranges));
            result.ranges.insert(end(result.ranges), begin(b.ranges), end(b.ranges));
            result = result.normalized();
        }
        return result;
    }

    static pair<seq_t, seq_t> split(const seq_t& seq, int i) {
        seq_t first, second;
        int index = 0;

        for (auto range : seq.ranges) {
            int l = index;
            int r = index + range.iterations * range.cnt - 1;
            if (i < l) {
                second.size += range.iterations * range.cnt;
                second.ranges.push_back(range);
            } else if (i > r) {
                first.size += range.iterations * range.cnt;
                first.ranges.push_back(range);
            } else {
                int left_iterations = (i - l) / range.cnt;
                int left_cnt = (i - l) % range.cnt;
                int right_iterations = (range.cnt * range.iterations - i + l) / range.cnt;
                int right_cnt = (range.cnt * range.iterations - i + l) % range.cnt;
                int color = (left_iterations & 1) ^ range.color;
                int right_color = (right_iterations & 1) ^ range.get_last_color() ^ 1;

                if (left_iterations > 0) {
                    first.ranges.push_back({range.color, range.cnt, left_iterations});
                    first.size += left_iterations * range.cnt;
                }
                if (left_cnt > 0) {
                    first.ranges.push_back({color, left_cnt, 1});
                    first.size += left_cnt;
                }
                if (right_cnt > 0) {
                    second.ranges.push_back({color, right_cnt, 1});
                    second.size += right_cnt;
                }
                if (right_iterations > 0) {
                    second.ranges.push_back({right_color, range.cnt, right_iterations});
                    second.size += right_iterations * range.cnt;
                }
            }
            index = r + 1;
        }
        auto answer = make_pair(first.normalized(), second.normalized());
        clog << "split " << seq << " " << i << "    ==>    " << answer.first << "  -  " << answer.second << endl;
        return answer;
    }

    range_t take_side(vector<range_t>& ranges, bool side) {
        range_t result;
        while (ranges.size() > 0) {
            range_t& range = side ? ranges.back() : ranges.front();

            int color = side ? range.get_last_color() : range.color;

            if (color == result.color || result.iterations == 0) {
                result.color = color;
                result.cnt += range.cnt;
                result.iterations = 1;

                if (range.iterations > 1) {
                    range.iterations -= 1;
                    range.color ^= side ^ 1;
                    break;
                } else {
                    ranges.erase(side ? (--end(ranges)) : begin(ranges));
                }
            } else {
                break;
            }
        }

        return result;
    }

    seq_t normalized() {
        seq_t answer;
        vector<range_t> ranges = this->ranges;

        range_t left = take_side(ranges, 0);
        range_t right = take_side(ranges, 1);
        seq_t::normalize(ranges);

        if (ranges.size() <= 1) {
            if (left.iterations > 0) {
                ranges.insert(begin(ranges), left);
            }
            if (right.iterations > 0) {
                ranges.insert(end(ranges), right);
            }
            answer = {size, sub_stripe_path, ranges};
        } else {
            answer = {0, 0, {}};
        }
        clog << "normalized " << *this << " => " << answer << " ( " << left << " ? " << right << " ) " << endl;
        return answer;
    }

    static void normalize(vector<range_t>& ranges) {
        vector<range_t> new_ranges;
        if (ranges.size() <= 1) {
            return;
        }
        for (const range_t& range : ranges) {
            seq_t::append(new_ranges, range);
        }
        ranges = new_ranges;
    }

    static void append(vector<range_t>& ranges, range_t range) {
        if (ranges.size() == 0) {
            ranges.push_back(range);
            return;
        }
        range_t last_range = ranges.back();
        if (last_range.iterations == 0) {
            ranges.pop_back();
            seq_t::append(ranges, range);
        } else if (range.iterations == 0) {
            return;
        } else if (last_range.get_last_color() != range.color) {
            if (last_range.cnt == range.cnt) {
                ranges.back().iterations += range.iterations;
            } else {
                ranges.push_back(range);
            }
        } else {
            range_t tmp_range = {last_range.get_last_color(), last_range.cnt + range.cnt, 1};
            ranges.pop_back();
            seq_t::append(ranges, {last_range.color, last_range.cnt, last_range.iterations - 1});
            seq_t::append(ranges, tmp_range);
            seq_t::append(ranges, {range.color ^ 1, range.cnt, range.iterations - 1});
        }
    }
};

ostream& operator<<(ostream& os, const seq_t& seq) {
    os << "seq {" << seq.size << ", " << seq.sub_stripe_path << ",";
    for (const auto& range : seq.ranges) {
        os << " " << range;
    }
    os << "} ";

    return os;
}


struct seg_tree_t {
    struct node_t {
        bool propagate;
        seq_t seq;
        int l, r;

        node_t() 
            : propagate(false), seq(), l(-1), r(-1) {
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
        nodes[root].propagate = true;
    }

    void modify(int ll, int rr, seq_t seq) {
        modify(ll, rr, 1, N, root, seq);
    }

    void modify(int ll, int rr, int l, int r, int v, seq_t seq) {
        expand(v, l, r);
        if (r < ll || rr < l) {
            return;
        }
            clog << "\tmodify " << ll << " " << rr << " " << l << " " << r << " " << v << " " << seq << endl;
        if (ll <= l && r <= rr) {
            nodes[v].seq = seq;
            nodes[v].propagate = true;
            expand(v, l, r);
            return;

        }
        int c = (l + r) / 2;
        auto seqs = seq_t::split(seq, c - max(ll, l) + 1);
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
        seq_t result = seq_t::compose(left, right);

        clog << "get " << ll << ", " << rr << ", " << l << ", " << r << ", " << v << " => " << result << endl;
        return result;
    }


    int create_node() {
        return cnt++;
    }

    void expand(int v, int l, int r) {
    //    clog << "expand " << v << ", "  << l << ", " << r << endl;

        if (nodes[v].l == -1 || nodes[v].r == -1) {
            nodes[v].l = create_node();
            nodes[v].r = create_node();
        }
        if (nodes[v].propagate && l != r) {
            nodes[v].propagate = false;
            int c = (l + r) / 2;
            auto seqs = seq_t::split(nodes[v].seq, c - l + 1);

            nodes[nodes[v].l].seq = seqs.first;
            nodes[nodes[v].l].propagate = true;

            nodes[nodes[v].r].seq = seqs.second;
            nodes[nodes[v].r].propagate = true;
        }
    }
};


vector<int> lefts, rights;
vector<int> parents, sizes;
vector<int> ltime, rtime, times;
vector<int> next_path;
vector<int> depths;

ostream& operator<<(ostream& os, seg_tree_t& tree) {
    for (int i = 1; i < tree.N; i++) {
        auto e = tree.get(ltime[i], ltime[i]);
        os << "<<<< " << e << " >>>> " << endl << endl;
    }
    return os;
}


void precalc(int v, int p = 0) {
    clog << "precalc " << v << " " << p << endl; 
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
    clog << "build_hld " << v << " " << p << endl;
    clog << "\t" << v << " =====>>>> " << next_path[v] << endl;
    static int time = 0;
    ltime[v] = ++time;
    times[time] = v;

    for (int u : { lefts[v], rights[v] }) {
        if (u != 0 && u != p) {
            if (u == lefts[v] || lefts[v] == 0) {
                next_path[u] = next_path[v];
            } else {
                next_path[u] = u;
            }
            build_hld(u, v);
        }
    }


    rtime[v] = time;
}

vector<pair<int, int>> get_up_segments(int& u, int& v) {
    vector<pair<int, int>> segments;
    while (next_path[u] != 0 && !inside(next_path[u], v)) {
        segments.push_back(make_pair(u, next_path[u]));
        u = parents[next_path[u]];
    }
    return segments;
}

vector<pair<int, int>> get_down_segments(int& u, int& v) {
    vector<pair<int, int>> segments;
    while (next_path[u] != 0 && !inside(next_path[u], v)) {
        segments.push_back(make_pair(next_path[u], u));
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

    int uu = u;
    int vv = v;

    auto up_segments = get_up_segments(u, v); // what?
    auto down_segments = get_down_segments(v, u);

    segments.insert(end(segments), begin(up_segments), end(up_segments));
    clog << "\tup_segments =";
    for (auto segment : up_segments) {
        clog << " <" << segment.first << ", " << segment.second << ">";
    }
    clog << endl;
    clog << "\tdown_segments =";
    for (auto segment : down_segments) {
        clog << " <" << segment.first << ", " << segment.second << ">";
    }
    clog << endl;
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
            down_segments.insert(begin(down_segments), make_pair(u, prev_v));
        }
    }
    segments.insert(end(segments), begin(down_segments), end(down_segments));

    clog << uu << " " << vv << " => ";
    for (const auto& segment : segments) {
        clog << "<"<< segment.first << " " << segment.second << ">, ";
    }
    clog << endl;

    return segments;
}

bool query(seg_tree_t& tree, int u, int v) {
    vector<pair<int, int> > segments = get_segments(u, v);

    seq_t answer = {0, true, {}};
    for (const auto& segment : segments) {

        if (ltime[segment.first] < ltime[segment.second]) {
            auto seq = tree.get(ltime[segment.first], ltime[segment.second]);
            answer = seq_t::compose(answer, seq);
        } else {
            auto seq = tree.get(ltime[segment.second], ltime[segment.first]);
            seq = seq_t::reverse(seq);
            answer = seq_t::compose(answer, seq);
        }
    }

    clog << "\tquery " << u << " " << v << " => " << answer << endl;

    bool ok = answer.sub_stripe_path;
    if (answer.ranges.size() == 0 || answer.ranges.size() >= 4 || (answer.size % 2) != 0) {
        return false;
    } else if (answer.ranges.size() == 1) {
        auto first = answer.ranges.front();
        return ok && (first.color == 1) && (first.iterations % 2 == 0);
    } else if (answer.ranges.size() == 2) {
        auto first = answer.ranges[0];
        auto second = answer.ranges[1];
        return ok && (first.color == 1) && (first.cnt == second.cnt) && ((first.iterations + second.iterations) % 2 == 0);
    } else if (answer.ranges.size() == 3) {
        auto first = answer.ranges[0];
        auto second = answer.ranges[1];
        auto third = answer.ranges[2];
        return ok && (first.color == 1) && (first.cnt == second.cnt) && (second.cnt == third.cnt) && ((first.iterations + second.iterations + third.iterations) % 2 == 0);
    }
    return false;
}

void colorize_path(seg_tree_t& tree, int u, int v, int k) {
    clog << "colorize_path " << u << " " << v << " " << k << endl;
    vector<pair<int, int> > segments = get_segments(u, v);

    int lca_uv = lca(u, v);
    int path_length = depths[v] + depths[u] - 2 * depths[lca_uv];

    vector<range_t> ranges = {range_t {1, k, path_length / k}};
    seq_t seq = {path_length, true, ranges};

    clog << "\tseq " << seq << endl;

    for (const auto& segment : segments) {
        int length = abs(ltime[segment.first] - ltime[segment.second]) + 1;
        auto seqs = seq_t::split(seq, length);
        if (ltime[segment.first] < ltime[segment.second]) {
            tree.modify(ltime[segment.first], ltime[segment.second], seqs.first);
        } else {
            clog << "\t\t Here" << endl;
            seqs.first = seq_t::reverse(seqs.first);
            tree.modify(ltime[segment.second], ltime[segment.first], seqs.first);
        }
        seq = seqs.second;
    }
}


int main() {
    ios::sync_with_stdio(false);

    int n;
    cin >> n;

    lefts.resize(n + 1);
    rights.resize(n + 1);
    parents.resize(n + 1);

    for (int i = 1; i <= n; i++) {
        int l, r;
        cin >> l >> r;

        if (l != 0) {
            parents[l] = i;
            lefts[i] = l;
        }

        if (r != 0) {
            parents[r] = i;
            rights[i] = r;
        }
    }

    ltime.resize(n + 1);
    rtime.resize(n + 1);
    times.resize(n + 1);
    sizes.resize(n + 1);
    depths.resize(n + 1);
    next_path.resize(n + 1);
    
    seg_tree_t tree(n + 1);

    precalc(1);
    build_hld(1);

    clog << endl << endl;
    clog << tree << endl;
    clog << endl << endl;

    int q;
    cin >> q;

    for (int i = 0; i < q; i++) {
        int type;
        cin >> type;
        if (type == 1) {
            int u, v;
            cin >> u >> v;

            cout << (query(tree, u, v) ? "Yes" : "No") << endl;
        } else {
            int u, v, k;
            cin >> u >> v >> k;

            colorize_path(tree, u, v, k);
        }
    }

    return 0;
}
