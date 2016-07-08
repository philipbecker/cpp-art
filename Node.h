#ifndef ART_NODE_H
#define ART_NODE_H


#include <cstdint>
#include <iostream>
#include <bitset>

union Key {
    int value;
    uint8_t chunks[sizeof(int)];
};

std::string print_key(Key key);

std::string print_key(Key key, unsigned depth);

const uint8_t EMPTY_MARKER = 48;

class Node {

public:
    virtual Node *insert(const Key &key, unsigned depth) = 0;

    virtual void insert(const uint8_t &key_byte, Node *node) = 0;

    virtual Node **find(const Key &key, unsigned depth) = 0;

    virtual Node **find(const uint8_t &key_byte) = 0;

    virtual void traverse(unsigned depth) = 0;

    virtual bool is_leaf();

    size_t size() {
        return count;
    }

    uint8_t count;
};


#endif //ART_NODE_H
