#include <iostream>
#include "adapt_radix_tree.h"

using namespace std;

int main() {
    /**
    art::adapt_radix_tree<int, int> art;
    auto p = art.insert(make_pair(5, 5));
    std::cout << p.second << std::endl;
    p = art.insert(make_pair(6, 6));
    std::cout << p.second << std::endl;
    p = art.insert(make_pair(7, 7));
    std::cout << p.second << std::endl;
    p = art.insert(make_pair(8, 8));
    std::cout << p.second << std::endl;
    p = art.insert(make_pair(9, 9));
    std::cout << p.second << std::endl;
    p = art.insert(make_pair(10, 10));
    std::cout << p.second << std::endl;
     */
//    art.insert(7);
//    art.insert(260);
//    art.insert(261);
//    art.insert(262);

    //cout << "TRAVERSE" << endl;
    //art.traverse();


    //for (auto &i : {5, 2, 1, 260, 262, 6, 263, 261, 8, 7, 10}) {
    //    cout << "Find(" << i << "): " << art.find(i) << endl;
    //}

    art::adapt_radix_tree<int, int> art;
    for (int i = 0; i < 1000; i++) {
        if (i % 3 == 0) {
            std::cout << i << ", ";
            auto p = art.insert(std::make_pair(i, i));
        }
    }

    for (int i = 0; i < 1000; i++) {
        if (i % 3 == 0) {
            std::cout << "find " << i << std::endl;
            auto p = art.find(i);
        } else {
            auto p = art.find(i);
        }
    }
    return 0;
}