#include <iostream>
#include <inttypes.h>
#include <queue>
#include <fstream>

uint64_t getPowerOfTwo(uint64_t number){
    uint64_t cnt = 0;
    while(number){
        cnt++;
        number >>= 1;
    }
    return cnt;
}

void do_solution(int test){

    uint64_t N, P;
    std::queue<uint64_t> tree;
    std::ifstream in;
    std::ofstream out;

    in.open("../tests/in/" + std::to_string(test));
    out.open("../tests/out/" + std::to_string(test));

    in >> N >> P;
    tree.push(P);
    for(uint64_t i = 0; i < N; i++){
        unsigned char operation;
        in >> operation;
        if(operation == '+'){
            uint64_t data;
            in >> data;
            tree.push(data);
            out << 0 << " " << tree.front() << std::endl;
        }else{
            tree.pop();
            out << getPowerOfTwo(tree.size()) - 1 << " " << tree.front() << std::endl;
        }
    }
    in.close();
    out.close();
    while (!tree.empty())
        tree.pop();
    
}

int main(){
    for(int i = 1; i <= 50; i++){
        system("clear");
        std::cout << i * 2 << "%" << std::endl;
        do_solution(i);
    }
    return 0;    
}