#include <iostream>
#include <random>
#include <algorithm>
#include <map>
#include "art/adaptive_radix_tree.h"
#include "art/radix_map.h"

struct Scientist {
    int id;
    std::string first_name;
    std::string last_name;

    Scientist() { }

    Scientist(int id, std::string first_name, std::string last_name)
            : id(id), first_name(first_name), last_name(last_name) { }
};

std::ostream &operator<<(std::ostream &os, const Scientist &s) {
    os << s.first_name << " " << s.last_name << " (" << s.id << ")";
    return os;
}

int main() {

    art::radix_map<int, std::string> radix_map;

    radix_map.insert(std::make_pair(4, "end"));

    std::cout << radix_map.begin()->first << std::endl;
    std::cout << radix_map.begin()->second << std::endl;
    std::cout << radix_map.cbegin()->first << std::endl;
    std::cout << radix_map.cbegin()->second << std::endl;
    std::cout << radix_map.rbegin()->first << std::endl;
    std::cout << radix_map.rbegin()->second << std::endl;

    radix_map.insert(std::make_pair(2, "is"));
    radix_map.insert(std::make_pair(1, "This"));
    radix_map.insert(std::make_pair(3, "the"));
    radix_map.insert(std::make_pair(5, "!!!"));

    for (auto &e : radix_map)
        std::cout << e.second << " ";
    std::cout << std::endl;

    for (const auto &e : radix_map)
        std::cout << e.second << " ";
    std::cout << std::endl;

    auto end = radix_map.rend();
    for (auto it = radix_map.rbegin(); it != end; ++it) {
        std::cout << it->second << " ";
    }
    std::cout << std::endl;

    auto cend = radix_map.cend();
    for (auto it = radix_map.cbegin(); it != cend; ++it) {
        std::cout << it->second << " ";
    }
    std::cout << std::endl;

    art::radix_map<unsigned, unsigned> map;
    map.emplace(1, 5);
    std::cout << map.begin()->first << std::endl;
    std::cout << map.begin()->second << std::endl;
    std::cout << map.cbegin()->first << std::endl;
    std::cout << map.cbegin()->second << std::endl;
    std::cout << map.rbegin()->first << std::endl;
    std::cout << map.rbegin()->second << std::endl;

    std::map<int, Scientist> std_map_2;
    art::radix_map<int, Scientist> radix_map_2;

    Scientist s1 {142124253, "Alan", "Turing"};
    Scientist s2 {799567878, "John", "Neumann"};

    radix_map_2.insert(std::pair<int, Scientist>(1, s1));
    radix_map_2.insert(std::pair<int, Scientist>(2, s2));

    for (auto &e : radix_map_2)
        std::cout << e.second.first_name << " " << e.second.last_name << std::endl;
    std::cout << std::endl;

    art::radix_map<int, Scientist *> radix_map_3;
    std::map<int, Scientist *> std_map_3;

    auto alan = new Scientist(142124253, "Alan", "Turing");
    auto john = new Scientist(799567878, "Alan", "Neumann");

    radix_map_3.insert(std::pair<int, Scientist *>(1, alan));
    radix_map_3.insert(std::pair<int, Scientist *>(2, john));

    for (auto &e : radix_map_2)
        std::cout << e.second.first_name << " " << e.second.last_name << std::endl;
    std::cout << std::endl;

    std::cout << "Memory footprint" << std::endl;
    std::cout << "Pair<int, int> \t\t" << sizeof(std::pair<int, int>) << std::endl;
    std::cout << "Node Type\t\t" << "Size" << std::endl;
    std::cout << "Node\t\t\t" << sizeof(art::adaptive_radix_tree<int, int>::_Node) << std::endl;
    std::cout << "Dummy Node\t\t" << sizeof(art::adaptive_radix_tree<int, int>::_Dummy_Node) << std::endl;
    std::cout << "Leaf\t\t\t" << sizeof(art::adaptive_radix_tree<int, int>::_Leaf) << std::endl;
    std::cout << "Node 4\t\t\t" << sizeof(art::adaptive_radix_tree<int, int>::_Node_4) << std::endl;
    std::cout << "Node 16\t\t\t" << sizeof(art::adaptive_radix_tree<int, int>::_Node_16) << std::endl;
    std::cout << "Node 48\t\t\t" << sizeof(art::adaptive_radix_tree<int, int>::_Node_48) << std::endl;
    std::cout << "Node 256\t\t" << sizeof(art::adaptive_radix_tree<int, int>::_Node_256) << std::endl;

    return 0;
}