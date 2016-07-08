//
// Created by philip on 06.07.16.
//

#ifndef ART_LEAF_H
#define ART_LEAF_H


#include "Node.h"

class Leaf : public Node {
private:
    Key key;

public:
    Leaf(Key key) : key(key) { }

    virtual Node *insert(const Key &key, unsigned depth) override {
        return this;
    }

    virtual void insert(const uint8_t &key_byte, Node *node) override {

    }

    virtual Node **find(const Key &key, unsigned depth) override {
        return nullptr;
    }

    virtual void traverse(unsigned depth) override {
        std::cout << std::string(depth+1, '-') << " Leaf " << this << ": " << key.value << std::endl;
    }

    virtual Node **find(const uint8_t &key_byte) override {
        return nullptr;
    }


    bool contains(const Key &key, unsigned depth) {
        return this->key.value == key.value;
    }

    bool is_leaf() override {
        return true;
    }

    Key& get_key() {
        return key;
    }
};


#endif //ART_LEAF_H
