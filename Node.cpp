//
// Created by philip on 06.07.16.
//

#include "Node.h"

bool Node::is_leaf() {
    return false;
}

std::string print_key(Key key, unsigned depth) {
    std::bitset<8> x(key.chunks[depth]);
    std::string output = x.to_string();
    return output;
}

std::string print_key(Key key) {
    std::string output;
    for (unsigned i = 0; i < sizeof(key.value); i++) {
        output += print_key(key, i) + ", ";
    }
    return output;
}

