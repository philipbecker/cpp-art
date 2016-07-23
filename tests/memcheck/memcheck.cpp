#include <random>
#include <map>
#include "art/radix_map.h"

int main() {
    art::radix_map<int, int> art;
    //std::map<int, int> map;

    const size_t elements = 10;
    for (int i = 0; i < elements; i++)
       art.insert(std::make_pair(i, 1));

    //art::radix_map<int, int> art2(art);
    //art::radix_map<int, int> art3(std::move(art));
    //art = std::move(art3);
    //art3 = art;

    //std::cout << art.size() << std::endl;
    //std::cout << art2.size() << std::endl;
    //std::cout << art3.size() << std::endl;

    for (int i = 0; i < elements; i++) {
        //art.erase(art.find(i));
        art.erase(i);
    }

    return 0;
}
