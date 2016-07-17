#include <iostream>
#include <random>
#include <algorithm>
#include <map>
#include "src/Adaptive_radix_tree.h"
#include "src/radix_map.h"

using namespace std;

int main() {

    std::cout << "Memory footprint" << std::endl;
    std::cout << "Node Type\t\t" << "Size" << std::endl;
    std::cout << "Dummy Node\t\t" << sizeof(art::Adaptive_radix_tree<int, int>::_Dummy_Node) << std::endl;
    std::cout << "Leaf\t\t\t" << sizeof(art::Adaptive_radix_tree<int, int>::_Leaf) << std::endl;
    std::cout << "Node 4\t\t\t" << sizeof(art::Adaptive_radix_tree<int, int>::_Node_4) << std::endl;
    std::cout << "Node 16\t\t\t" << sizeof(art::Adaptive_radix_tree<int, int>::_Node_16) << std::endl;
    std::cout << "Node 48\t\t\t" << sizeof(art::Adaptive_radix_tree<int, int>::_Node_48) << std::endl;
    std::cout << "Node 256\t\t" << sizeof(art::Adaptive_radix_tree<int, int>::_Node_256) << std::endl;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    std::vector<std::pair<int64_t, int64_t>> v;
    for (int i = 2; i < 10; i++) {
        auto candidate = dis(gen);
        v.emplace_back(i, i);
    }

    art::radix_map<int64_t, int64_t> map;

    for (auto &e: v)
        map.insert(e);

    int sum = 0;
    for (auto &e: v)
        sum += map.find(e.first)->second;

    std::cout << sum << std::endl;


    std::cout << "forward" << std::endl;
    int count = 0;
    for (auto it = map.begin(), end = map.end(); it != end; ++it) {
        std::cout << (*it).second << ", ";
        if (count > 20) break;
        count++;
    }

    std::cout << std::endl << "backward" << std::endl;
    count = 0;
    for (auto it = map.rbegin(), end = map.rend(); it != end; ++it) {
        std::cout << (*it).second << ", ";
        if (count > 20) break;
        count++;
    }

    /**
    art::radix_map<unsigned, unsigned> art;
    std::map<unsigned, unsigned> std_map;

    std::cout << "---------------" << std::endl;
    std::cout << "Begin of art: " << (art.begin() == art.end()) << std::endl;
    std::cout << "RBegin of art: " << (art.rbegin() == art.rend()) << std::endl;
    std::cout << "---------------" << std::endl;
*/

    /**
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
*/

    return 0;
}