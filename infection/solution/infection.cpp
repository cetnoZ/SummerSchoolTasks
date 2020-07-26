#include <iostream>
#include <inttypes.h>
#include <queue>
#include <fstream>
#include <algorithm>
#include <memory.h>
std::vector<uint64_t> lists;

const uint32_t maxn = 30000 + 7;

bool check[maxn] = {false};

std::vector<std::vector<std::pair<uint32_t, uint32_t>>> tree;

void dfs(uint64_t v, uint64_t sum = 0){
    check[v] = true;
    for(uint32_t i = 0; i < tree[v].size(); i++)
        if(!check[tree[v][i].first])
            dfs(tree[v][i].first, sum + tree[v][i].second);
    if(tree[v].size() == 1)
        lists.push_back(sum);
}


void do_solution(int test){

    uint64_t N, E, S;
    std::ifstream in;
    std::ofstream out;

    in.open("../tests/in/" + std::to_string(test));
    out.open("../tests/out/" + std::to_string(test));
    std::vector<uint64_t> virus_array;

    in >> E >> N >> S;
    tree.resize(E + 2);
    for(uint32_t i = 0; i < N; i++){
        uint64_t cost;
        in >> cost;
        virus_array.push_back(cost);
    }
    for(uint32_t i = 0; i < E; i++){
        uint32_t u, v, c;
        in >> u >> v >> c;
        tree[u].push_back(std::make_pair(v, c));
        tree[v].push_back(std::make_pair(u, c));
    }

    std::sort(virus_array.begin(), virus_array.end(), std::greater<int>());
    dfs(S);
    std::sort(lists.begin(), lists.end());

    uint64_t res = 0;
    for(uint32_t i = 0; i < std::min(lists.size(), virus_array.size()); i++){
        if(lists[i] < virus_array[i]){
            res += virus_array[i] - lists[i];
        }
    }
    out << res << std::endl;
    in.close();
    out.close();
    
    tree.clear();
    lists.clear();
    memset(check, 0, maxn);
}

int main(){
    for(int i = 1; i <= 50; i++){
        system("clear");
        std::cout << i * 2 << "%" << std::endl;
        do_solution(i);
    }
    return 0;    
}