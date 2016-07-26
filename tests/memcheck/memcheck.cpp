#include <random>
#include <map>
#include "art/radix_map.h"

int main() {
    art::radix_map<int, int> art;
    //std::map<int, int> map;

    const size_t elements = 2;
    for (int i = 0; i < elements; i++) {
        //art.insert(std::make_pair(i, 1));
        art.emplace(i, 1);
    }

    for (int i = 0; i < elements; i++) {
        //art.erase(art.find(i));
        art.erase(i);
    }

    return 0;
}
