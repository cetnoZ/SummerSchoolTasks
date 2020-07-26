#include <iostream>
#include <time.h>
#include <random>
#include <inttypes.h>
#include <string>
#include <fstream>
#include <memory.h>
#include <vector>
#include <algorithm>

const uint32_t maxn = 1'000'000;
const uint32_t maxp = 1'000'000'000;
const uint32_t maxa = 1'000'000'000;

const uint32_t number_of_tests = 50;

int balance = 0;

uint32_t get_random_a(){
    return (rand() * rand()) % maxa + 1;
}

void make_tests()
{
    for(int32_t i = 1; i <= number_of_tests; i++){
        system("clear");
        std::cout << i * 2 << "%\n";


        std::ofstream out_file;
        out_file.open(("../tests/in/" + std::to_string(i)).c_str());

        uint32_t N = (rand() * rand()) % maxn + 1;
        uint32_t P = get_random_a();
        std::vector<uint32_t> array_of_tree;

        out_file << N << " " << P << "\n";

        uint32_t shift = 31;
        uint32_t new_P = 1;
        for(uint32_t i = 0; i < N; i++){
            new_P += (rand() % shift + 1);
            array_of_tree.push_back(new_P);
        }
        std::random_shuffle(array_of_tree.begin(), array_of_tree.end());
        for(uint32_t j = 0; j < N; j++){
           
            if((rand() % 5) == 0 && balance > 2){
                balance--;
                out_file << "-\n";
            }else{
                balance++;
                out_file << "+ " << array_of_tree[j] << "\n";
            }
        }
        balance = 0;
        out_file.close();
        
    }
}

int main(){
    srand(time(NULL));
    make_tests();
}