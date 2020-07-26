#include <iostream>
#include <inttypes.h>
#include <queue>
#include <fstream>
#include <limits>
#include <vector>
#include <algorithm>
std::vector<int64_t> lists;

const int64_t maxn = 30000 + 7;

bool check[maxn] = {false};

std::vector<std::vector<std::pair<int64_t, int64_t>>> tree;

void dfs(int64_t v, int64_t sum = 0){
    check[v] = true;
    for(int64_t i = 0; i < tree[v].size(); i++)
        if(!check[tree[v][i].first])
            dfs(tree[v][i].first, sum + tree[v][i].second);
    if(tree[v].size() == 1)
        lists.push_back(sum);
}


int main(){
	int64_t N, E, S;
    std::vector<int64_t> virus_array;

    std::cin >> E >> N >> S;
    tree.resize(E + 2);
    for(int64_t i = 0; i < N; i++){
        int64_t cost;
        std::cin >> cost;
        virus_array.push_back(cost);
    }
    for(int64_t i = 0; i < E; i++){
        int64_t u, v, c;
        std::cin >> u >> v >> c;
        tree[u].push_back(std::make_pair(v, c));
        tree[v].push_back(std::make_pair(u, c));
    }

    std::sort(virus_array.begin(), virus_array.end(), std::greater<int>());
    dfs(S);
    std::sort(lists.begin(), lists.end());

    int64_t res = 0;
    for(int64_t i = 0; i < std::min(lists.size(), virus_array.size()); i++){
        if(lists[i] < virus_array[i]){
            res += virus_array[i] - lists[i];
        }
    }
    std::cout << res << std::endl;
    return 0;    
}