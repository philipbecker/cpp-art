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
    using std::cout;
    using std::endl;

    art::radix_map<uint32_t, uint32_t> m;
    for (uint32_t i = 1; i < 2500; i++) {
        auto res = m.insert(std::make_pair(i << 16, i));
        if (res.second)
            cout << res.first->first << " -> " << res.first->second << endl;
        else
            cout << "no success with " << i << endl;
    }

    cout << "size: " << m.size() << endl;

    for (uint32_t i = 1; i < 2500; i++) {
        auto it = m.erase(i<< 16);
        cout << "erase " << i << " was " << it << endl;
    }

    cout << "size: " << m.size() << endl;

    for (uint32_t i = 1; i < 250; i++) {
        auto res = m.insert(std::make_pair(i << 16, i));
        if (res.second)
            cout << res.first->first << " -> " << res.first->second << endl;
        else
            cout << "no success with " << i << endl;
    }


    cout << "Memory footprint" << endl;
    cout << "Pair<int, int> \t\t" << sizeof(std::pair<int, int>) << endl;
    cout << "Node Type\t\t" << "Size" << endl;
    cout << "Node\t\t\t" << sizeof(art::adaptive_radix_tree<int, int>::_Node) << endl;
    cout << "Dummy Node\t\t" << sizeof(art::adaptive_radix_tree<int, int>::_Dummy_Node) << endl;
    cout << "Leaf\t\t\t" << sizeof(art::adaptive_radix_tree<int, int>::_Leaf) << endl;
    cout << "Node 4\t\t\t" << sizeof(art::adaptive_radix_tree<int, int>::_Node_4) << endl;
    cout << "Node 16\t\t\t" << sizeof(art::adaptive_radix_tree<int, int>::_Node_16) << endl;
    cout << "Node 48\t\t\t" << sizeof(art::adaptive_radix_tree<int, int>::_Node_48) << endl;
    cout << "Node 256\t\t" << sizeof(art::adaptive_radix_tree<int, int>::_Node_256) << endl;

    return 0;
}