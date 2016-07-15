#include <iostream>
#include <random>
#include <algorithm>
#include <map>
#include "src/Adaptive_radix_tree.h"
#include "src/radix_map.h"

using namespace std;

int main() {
    /*
    art::Adaptive_radix_tree<int, int> art;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-1000000, 1000000);

    std::vector<int> data;
    for (int i = 0; i < 10000; i++) {
        auto candidate = dis(gen);
        data.push_back(candidate);
    }

    std::cout << "Begin of vector: " << *data.begin() << std::endl;
    std::cout << data.back() << std::endl;
    std::cout << "End of vector: " << *data.end() << std::endl;

    for (auto &d : data)
        art.insert(std::make_pair(d, d));

*/
    art::Adaptive_radix_tree<unsigned , unsigned> art2;

    for (unsigned i = 1; i < 8; i += 1)
        art2.insert(std::make_pair(i, i));

    std::cout << "---------------" << std::endl;
    std::cout << "Begin of art: " << art2.begin()->second << std::endl;
    std::cout << "RBegin of art: " << art2.rbegin()->second << std::endl;
    std::cout << "---------------" << std::endl;

    for (auto &&item : art2)
        std::cout << item.first <<  " < ";
    std::cout << std::endl;

    for (auto it2 = art2.rbegin(), end = art2.rend(); it2 != end; it2++)
        std::cout << it2->first << " > ";
    std::cout << std::endl;

    art::radix_map<unsigned, unsigned> art;
    std::map<unsigned, unsigned> std_map;

    std::cout << "---------------" << std::endl;
    std::cout << "Begin of art: " << (art.begin() == art.end()) << std::endl;
    std::cout << "RBegin of art: " << (art.rbegin() == art.rend()) << std::endl;
    std::cout << "---------------" << std::endl;

    for (unsigned i = 1; i < 10; i++) {
        art.insert(std::pair<unsigned, unsigned>(i, i));
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