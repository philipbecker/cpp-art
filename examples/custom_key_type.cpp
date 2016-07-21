#include <iostream>
#include "art/radix_map.h"

struct Scientist {
    int id;
    std::string first_name;
    std::string last_name;
};

std::ostream &operator<<(std::ostream& os, const Scientist &s) {
    os << s.first_name << " " << s.last_name << " (" << s.id << ")";
    return os;
}

namespace art {
    template<>
    struct key_transform<Scientist> {
        int operator()(const Scientist &s) const noexcept {
            // Here we just use the id as the key (needs to be unique).
            // An integer is not binary comparable, so we need
            // to transform the id itself. There is already a key_transform
            // for all types of integers in the art namespace.
            return key_transform<int>()(s.id);
        }
    };
}

int main() {
    art::radix_map<Scientist, int> map = {
            {{1, "Alan", "Turing"},  10},
            {{-5, "John", "Neumann"}, 20}
    };

    for (auto &e : map)
        std::cout << e.first << ", " << e.second << std::endl;

    // Output:
    // John Neumann (-5), 20        
    // Alan Turing (1), 10

    return 0;
}