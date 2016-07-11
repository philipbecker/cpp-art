//
// Created by philip on 06.07.16.
//

#ifndef ART_LEAF_H
#define ART_LEAF_H

#include "node.h"

namespace art
{

    class _leaf : public node {
    private:
        Key key;

    public:
        _leaf(Key key) : key(key) { }

        virtual node *insert(const Key &key, unsigned depth) override {
            return this;
        }

        virtual void insert(const uint8_t &key_byte, node *n) override {

        }

        virtual void traverse(unsigned depth) override {
            std::cout << std::string(depth + 1, '-') << " _leaf " << this << ": " << key.value << std::endl;
        }

        virtual node **find(const uint8_t &key_byte) override {
            return nullptr;
        }


        bool contains(const Key &key, unsigned depth) {
            return this->key.value == key.value;
        }

        bool is_leaf() override {
            return true;
        }

        size_t max_size() const override {
            return 1;
        }

        virtual uint8_t get_type() const override {
            return 0;
        }

        Key &get_key() {
            return key;
        }
    };
}

#endif //ART_LEAF_H
