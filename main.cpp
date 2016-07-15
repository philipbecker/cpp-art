#include <iostream>
#include <random>
#include <algorithm>
#include <map>
#include "src/Adaptive_radix_tree.h"
#include "src/radix_map.h"

using namespace std;

int main() {
    art::radix_map<unsigned, unsigned> art;
    std::map<unsigned, unsigned> std_map;

    std::cout << "---------------" << std::endl;
    std::cout << "Begin of art: " << (art.begin() == art.end()) << std::endl;
    std::cout << "RBegin of art: " << (art.rbegin() == art.rend()) << std::endl;
    std::cout << "---------------" << std::endl;

    for (unsigned i = 1; i < 10; i++) {
        auto p = art.insert(std::pair<unsigned, unsigned>(i, i));
        std::cout << "insertion of " << i << (p.second ? " successful" : " failed") << std::endl;
        std::cout << "iterator value " << p.first->second << std::endl;
        while (! p.first.parents.empty()) {
            auto s = p.first.parents.top();
            p.first.parents.pop();
            std::cout << s.first->get_type() <<  " position: " << s.second << std::endl;
        }

        std_map.insert(std::pair<unsigned, unsigned>(i, i));
    }

    std::cout << "---------------" << std::endl;
    std::cout << "Begin of art: " << art.begin()->second << std::endl;
    std::cout << "RBegin of art: " << art.rbegin()->second << std::endl;
    std::cout << "Begin of stdmap: " << std_map.begin()->second << std::endl;
    std::cout << "RBegin of stdmap: " << std_map.rbegin()->second << std::endl;
    std::cout << "---------------" << std::endl;


    return 0;
}