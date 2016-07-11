//
// Created by philip on 08.07.16.
//

#ifndef REFERENCE_NODE4_H
#define REFERENCE_NODE4_H

#include <array>
#include "_node.h"
#include "_leaf.h"
#include "node_16.h"

namespace art
{
    class node_4 : public _node {
    public:
        std::array<uint8_t, 4> keys{};
        std::array<_node *, 4> children{};

        node_4(_leaf *leaf, unsigned depth) {
            Key key = leaf->get_key();
            keys[0] = key.chunks[depth];
            children[0] = leaf;

            _count = 1;
        }



        virtual uint16_t max_size() const override {
            return 4;
        }

        virtual _node *insert(const Key &key, unsigned depth) override {
            return nullptr;
        }

        virtual void insert(const uint8_t &key_byte, _node *node) override {
            unsigned pos = 0;
            for (; pos < _count && keys[pos] < key_byte; pos++);
            if (pos != _count) {
                std::move(keys.begin() + pos, keys.begin() + _count, keys.begin() + pos + 1);
                std::move(children.begin() + pos, children.begin() + _count, children.begin() + pos + 1);
            }
            keys[pos] = key_byte;
            children[pos] = node;
            _count++;
        }

        virtual _node **find(const uint8_t &key_byte) override {
            unsigned pos = 0;
            for (; pos < _count && keys[pos] < key_byte; pos++);

            if (keys[pos] == key_byte)
                return &children[pos];
            return nullptr;
        }

        virtual void traverse(unsigned depth) override {
            for (size_t i = 0; i < _count; i++)
                children[i]->traverse(depth + 1);
        }

        virtual node_type get_type() const override {
            return node_type::node_4_t;
        }

    };
}


#endif //REFERENCE_NODE4_H
