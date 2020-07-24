#include <iostream>
#include <time.h>
#include <random>
#include <inttypes.h>
#include <string>
#include <fstream>

const uint32_t maxn = 1'000'000;
const uint32_t maxp = 1'000'000'000;
const uint32_t maxa = 1'000'000'000;

const uint32_t number_of_tests = 50;

int balance = 0;



void make_tests()
{
    for(int32_t i = 1; i <= number_of_tests; i++){
        system("clear");
        std::cout << i*2 << "%\n";

        std::ofstream out_file;
        out_file.open(("../tests/in/" + std::to_string(i)).c_str());
        uint32_t N = (rand() * rand()) % maxn + 1;
        uint32_t P = N * (rand() % 7 + 3);
        out_file << N << " " << P << "\n";

        uint32_t shift = ((P / N) + 1) * 2;

        for(uint32_t j = 0; j < N; j++){
           
            if((rand() % 5) == 0 && balance > 2){
                balance--;
                out_file << "-\n";
            }else{
                balance++;
                uint32_t new_P = P - (rand() % shift + 1);
                out_file << "+ " << new_P << "\n";
                P = new_P;
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