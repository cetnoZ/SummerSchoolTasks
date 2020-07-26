#include <iostream>
#include <time.h>
#include <random>
#include <inttypes.h>
#include <string>
#include <fstream>
#include <memory.h>
#include <vector>
#include <algorithm>

const uint32_t maxn = 300'00;
const uint32_t maxp = 1'000'000'000;
const uint32_t maxa = 1'000'000'000;

const uint32_t number_of_tests = 50;

uint32_t get_random_a(){
    return (rand() * rand()) % maxa + 1;
}
uint32_t get_random_n(){
    return (rand() * rand()) % maxn + 1;
}

void make_tests()
{
    for(int32_t i = 1; i <= number_of_tests; i++){
        system("clear");
        std::cout << i * 2 << "%\n";
        std::vector<std::vector<int>> tree(maxn);

        std::ofstream out_file;
        out_file.open(("../tests/in/" + std::to_string(i)).c_str());

        uint32_t N = (rand() * rand()) % maxn + 1;
        uint32_t E = (rand() * rand()) % maxn + 1;
        uint32_t S = get_random_a() % E + 1;
        std::vector<uint32_t> array_of_tree;

        out_file << E << " " << N << " " << S << "\n";
        for(uint32_t i = 0; i < N; i++)
            out_file << get_random_a() << " ";

        for(int i = 0; i < E; i++){
            uint32_t edge_cost = get_random_a();
            while (true)
            {    
                uint32_t u = (rand() * rand() % (E + 1)) + 1;
                uint32_t v = (rand() * rand() % (E + 1)) + 1;
                if(u == v)
                    continue;
                if(!tree[u].empty() && std::find(std::begin(tree[u]), std::end(tree[u]), v) == std::end(tree[u])){
                    continue;
                }
                tree[u].push_back(v);
                tree[v].push_back(u);
                out_file << u << " " << v << " " << edge_cost << std::endl;
                break;
            }
        }
        out_file << "\n";
        out_file.close();
        
    }
}

int main(){
    srand(time(NULL));
    make_tests();
}