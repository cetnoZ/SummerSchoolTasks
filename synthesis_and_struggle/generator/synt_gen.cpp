#include "testlib.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <ctime>
#include <climits>
#include <cassert>
#include <vector>
#include <queue>
#include <stack>
#include <deque>
#include <set>
#include <map>
#include <bitset>
#include <utility>
#include <time.h>
#include <algorithm>

using namespace std; 

#define forn(i, n) for (int i = 0; i < int(n); i++)


const uint32_t maxn = 100;
const uint32_t maxp = 1'000'00;
const uint32_t maxa = 1'000'00;

const uint32_t number_of_tests = 1;

int argc;
char **argv;

void print_tree(int N, std::ofstream& out){
    
    registerGen(argc, argv, 1);

    int n = N;
    int t = 0;

    vector<int> p(n);
    forn(i, n)
        if (i > 0)
            p[i] = rnd.wnext(i, t);

    vector<int> perm(n);
    forn(i, n)
        perm[i] = i;
    shuffle(perm.begin() + 1, perm.end());
    vector<pair<int,int> > edges;

    for (int i = 1; i < n; i++)
        if (rnd.next(2))
            edges.push_back(make_pair(perm[i], perm[p[i]]));
        else
            edges.push_back(make_pair(perm[p[i]], perm[i]));

    shuffle(edges.begin(), edges.end());

    for (int i = 0; i + 1 < n; i++)
        out << edges[i].first + 1 << " " <<edges[i].second + 1 << "\n";
        //printf("%d %d\n", edges[i].first + 1, edges[i].second + 1);
}

void make_tests()
{
    for(int32_t i = 1; i <= number_of_tests; i++){
        system("clear");
registerGen(argc, argv, 1);
        std::ofstream out_file;
        out_file.open(("../tests/in/" + std::string(argv[1])).c_str());

        uint32_t N = rnd.next(2, 100);
        uint32_t K = rnd.next(0, 100000);
        std::vector<uint32_t> array_of_tree;

        out_file << N << " " << K << "\n";

        std::vector<int32_t> tmp;
        for(uint32_t i = 0; i < N - 1; i++)
            tmp.push_back(rnd.next(0, 100000));
        std::sort(tmp.begin(), tmp.end());
        for(const auto& item: tmp)
            out_file << item << " ";
        out_file << "\n";
        print_tree(N, out_file);

        /*for(int i = 0; i < N - 1; i++){
            while (true)
            {    
                uint32_t u = (rand()  % (N)) + 1;
                uint32_t v = (rand() * rand() % (N)) + 1;
                if(u == v)
                    continue;
                if(tree[u].size() != 0 && std::find(std::begin(tree[u]), std::end(tree[u]), v) != std::end(tree[u])){
                    continue;
                }
                
                tree[u].push_back(v);
                tree[v].push_back(u);
            
                cycle_exists = false;
                
                is_cycle(u);
                if(cycle_exists){
                    tree[u].pop_back();
                    tree[v].pop_back();
                    continue;
                }

                out_file << u << " " << v << std::endl;
                break;
            }
        }*/
        out_file << "\n";
        out_file.close();
    }
}

int main(int argc, char **argv){
    ::argc = argc;
    ::argv = argv;
    make_tests();
}