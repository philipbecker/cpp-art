#include <iostream>
#include <random>
#include <algorithm>
#include <map>
#include "art/adaptive_radix_tree.h"
#include "art/radix_map.h"


int main() {

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