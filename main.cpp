#include <iostream>
#include "adapt_radix_tree.h"

using namespace std;

int main() {
    art::adapt_radix_tree<int, int> art;

    for (int i = 0; i < 10000; i += 3)
        art.insert(std::make_pair(i, i));


    art::adapt_radix_tree<int, int>::iterator it = art.begin();
    for (; it != art.end(); ++it) {
        std::cout << *it << std::endl;
    }
    return 0;
}