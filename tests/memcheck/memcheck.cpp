#include <random>
#include "../../src/radix_map.h"

int main() {
    art::radix_map<int, int> art;

    for (int i = 0; i < 100000; i++)
       art.insert(std::make_pair(i, 1));

    long sum = 0;
    for (auto &&element : art)
        sum += element.second;

    std::cout << "SUM: " << sum << std::endl;

    for (int i = 0; i < 100000; i++)
        art.erase(i);

    return 0;
}
