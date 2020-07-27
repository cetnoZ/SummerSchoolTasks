#include <iostream>
#include <time.h>
#include <random>
#include <inttypes.h>
#include <string>
#include <fstream>
#include <memory.h>
#include <vector>
#include <algorithm>

const uint32_t maxn = 100000;
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

        std::ofstream out_file;
        out_file.open(("../tests/in/" + std::to_string(i)).c_str());

        uint32_t N = (rand() * rand()) % maxn + 1;
        uint32_t M = (rand() * rand()) % maxn + 1;

        std::vector<uint32_t> first_array;
        std::vector<uint32_t> second_array;
        

        out_file << N << " " << M << "\n";
        for(uint32_t i = 0; i < N; i++)
            first_array.push_back(get_random_a());
        
        for(uint32_t i = 0; i < M; i++)
            second_array.push_back(get_random_a());

        std::sort(first_array.begin(), first_array.end());
        std::sort(second_array.begin(), second_array.end());

        for(const auto& item: first_array)
            out_file << item << " ";
        out_file << "\n";

        for(const auto& item: second_array)
            out_file << item << " ";
        out_file << "\n";
        
        out_file.close();
        
    }
}

int main(){
    srand(time(NULL));
    make_tests();
}