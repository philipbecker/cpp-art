#include <iostream>
#include <random>
#include <algorithm>
#include <map>
#include "src/Adaptive_radix_tree.h"
#include "src/radix_map.h"

using namespace std;

struct custom_transform {
    int64_t operator()(const int64_t &key) const noexcept {
        return art::is_big_endian() ? key : art::byte_swap(key);
    }
};

struct Employee {
    uint64_t id;
    std::string first_name;
    std::string last_name;

};

std::ostream &operator<<(std::ostream& os, const Employee &e) {
    os << e.first_name << " " << e.last_name << " (" << e.id << ")";
    return os;
}

namespace art {
    template<>
    struct key_transform<Employee> {
        uint64_t operator()(const Employee &e) const noexcept {
            return e.id;
        }
    };
}

int main() {
    art::radix_map<int64_t, int64_t, custom_transform> map;

    map.insert(std::pair<int64_t, int64_t>(-2, 5));
    map.insert(std::pair<int64_t, int64_t>(0, 5));
    map.insert(std::pair<int64_t, int64_t>(7, 25));

    for (auto &e : map)
        std::cout << e.first << ", " << e.second << std::endl;

    Employee e1{1, "Alan", "Turing"};
    Employee e2{2, "John", "Neumann"};
    art::radix_map<int, int> employee_map;

    employee_map[1] = 10;
    employee_map.insert(std::pair<int, int>(e2.id, 20));

    for (auto &e : employee_map)
        std::cout << e.first << ", " << e.second << std::endl;

    std::cout << "Memory footprint" << std::endl;
    std::cout << "Node Type\t\t" << "Size" << std::endl;
    std::cout << "Dummy Node\t\t" << sizeof(art::Adaptive_radix_tree<int, int>::_Dummy_Node) << std::endl;
    std::cout << "Leaf\t\t\t" << sizeof(art::Adaptive_radix_tree<int, int>::_Leaf) << std::endl;
    std::cout << "Node 4\t\t\t" << sizeof(art::Adaptive_radix_tree<int, int>::_Node_4) << std::endl;
    std::cout << "Node 16\t\t\t" << sizeof(art::Adaptive_radix_tree<int, int>::_Node_16) << std::endl;
    std::cout << "Node 48\t\t\t" << sizeof(art::Adaptive_radix_tree<int, int>::_Node_48) << std::endl;
    std::cout << "Node 256\t\t" << sizeof(art::Adaptive_radix_tree<int, int>::_Node_256) << std::endl;

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