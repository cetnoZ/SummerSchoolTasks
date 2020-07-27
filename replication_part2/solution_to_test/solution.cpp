#include <iostream>
#include <inttypes.h>
#include <queue>
#include <fstream>
#include <limits>

const uint32_t maxn = 4e5 + 7;
constexpr uint32_t inf = (1 << 31);

class Heap{
    uint32_t mas[maxn];
    uint32_t count;
public:
    explicit Heap(){
        count = 0;
        mas[0] = inf;
        for(uint32_t i = 1; i < maxn; i++)
            mas[i] = 0;
    }
    ~Heap(){}

    uint32_t push(const uint32_t &num){
        mas[++count] = num;
        uint32_t j = count / 2;//parent
        uint32_t i = count;
        uint32_t operation_cnt = 0;
        while(mas[j] < mas[i])
        {
            std::swap(mas[j], mas[i]);
            i = j;
            j = i / 2;
            operation_cnt++;
        }
        return operation_cnt;
    }

    uint32_t pop(){
        mas[1] = mas[count];
        mas[count--] = 0;
        uint32_t operation_cnt = 0;
        uint32_t i = 1;
        uint32_t j = i * 2;
        while(mas[i] < mas[j] || mas[i] < mas[j + 1]){
            operation_cnt++;

            if(mas[i] < mas[j] && mas[i] < mas[j + 1]){
                
                if(mas[j] > mas[j + 1]){
                    std::swap(mas[j], mas[i]);
                    i = j;
                    j = i * 2;
                }else{
                    std::swap(mas[j + 1], mas[i]);
                    i = j + 1;
                    j = i * 2;
                }
                continue;
            }

            if(mas[i] < mas[j]){
                std::swap(mas[j], mas[i]);
                i = j;
                j = i * 2;
                continue;
            }
            if(mas[i] < mas[j + 1]){
                std::swap(mas[j + 1], mas[i]);
                i = j + 1;
                j = i * 2;
                continue;
            }
        }
        return operation_cnt;
    }

    uint32_t top(){
        return mas[1];
    }
};

int main(){
	uint64_t N, P;
    Heap tree;
    std::cin >> N >> P;
    tree.push(P);
    for(uint64_t i = 0; i < N; i++){
        unsigned char operation;
        std::cin >> operation;
        if(operation == '+'){
            uint64_t data;
            std::cin >> data;
            std::cout << tree.push(data) << " " << tree.top() << std::endl;
        }else{
            std::cout << tree.pop() << " " << tree.top() << std::endl;
        }
    }

    return 0;    
}