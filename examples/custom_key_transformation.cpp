#include <iostream>
#include "art/radix_map.h"

// Signed integer key transformation without flipping the sign bit,
// which is the most significant bit. This is set to 1 for negative
// values and 0 for positive values. This means, when comparing their
// binary representation, negative values are now larger than
// positive ones. So, this is kind of an "incorrect" transformation,
// but it shows the intricacies of comparing at bit-level.
struct custom_transform {
    int operator()(const int &key) const noexcept {
        return art::is_big_endian() ? key : art::byte_swap(key);
    }
};

int main() {
    art::radix_map<int, int, custom_transform> map;
    map.insert(std::pair<int, int>(-2, 6));
    map.insert(std::pair<int, int>(0, 5));
    map.insert(std::pair<int, int>(7, 25));

    for (auto &e : map)
        std::cout << e.first << " -> " << e.second << std::endl;

    // Output
    // 0 -> 5
    // 7 -> 25
    // -2 -> 6

    return 0;
}
