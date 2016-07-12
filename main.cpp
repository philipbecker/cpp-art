#include <iostream>
#include "adapt_radix_tree.h"

using namespace std;

int main() {
    art::adapt_radix_tree<int, int> art;

    for (int i = -3; i < 4; i += 1)
        art.insert(std::make_pair(i, i));

    art::adapt_radix_tree<int, int>::iterator it = art.begin();
    for (; it != art.end(); ++it) {
        std::cout << *it << std::endl;
    }

    art::adapt_radix_tree<unsigned , unsigned> art2;

    for (unsigned i = 0; i < 8; i += 1)
        art2.insert(std::make_pair(i, i));

    for (auto &&item : art2)
        std::cout << item << std::endl;

    return 0;
}