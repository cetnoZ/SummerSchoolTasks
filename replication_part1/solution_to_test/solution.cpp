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

int main(){
	uint64_t N, P;
    std::queue<uint64_t> tree;
    std::cin >> N >> P;
    tree.push(P);
    for(uint64_t i = 0; i < N; i++){
        unsigned char operation;
        std::cin >> operation;
        if(operation == '+'){
            uint64_t data;
            std::cin >> data;
            tree.push(data);
            std::cout << 0 << " " << tree.front() << std::endl;
        }else{
            tree.pop();
            std::cout << getPowerOfTwo(tree.size()) - 1 << " " << tree.front() << std::endl;
        }
    }

    return 0;    
}