#include <vector>
#include <set>

using namespace std;

struct range_t {
    int color;
    int cnt;
    int iterations;
};

struct seq_t {
    int size;
    bool sub_stripe_path;
    vector<range_t> ranges;

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
