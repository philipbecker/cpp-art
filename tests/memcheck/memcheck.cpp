#include <random>
#include "../../src/radix_map.h"

int main() {
    art::radix_map<int, int> art;

    for (int i = 0; i < 100000; i++)
       art.insert(std::make_pair(i, 1));

    art::radix_map<int, int> art2(art);
    art::radix_map<int, int> art3(std::move(art));
    art = std::move(art3);
    art3 = art;

    std::cout << art.size() << std::endl;
    std::cout << art2.size() << std::endl;
    std::cout << art3.size() << std::endl;

    for (int i = 0; i < 10; i++)
        art.erase(i);



    return 0;
}
