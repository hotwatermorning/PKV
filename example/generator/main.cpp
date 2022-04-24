#include "../../include/PKV/PKV.h"
#include <random>
#include <iostream>

int main()
{
    std::mt19937 gen(0x1234ABCD);
    std::uniform_int_distribution<std::uint32_t> dist;

    // generate 100 keys
    for(int i = 0; i < 100; ++i) {
        auto key = hwm::PKV::generateKey(dist(gen));

        std::cout << key << std::endl;

//        if(hwm::PKV::isValidKey(key) == false) {
//            std::cout << "Invalid." << std::endl;
//            return 1;
//        }
    }
}
