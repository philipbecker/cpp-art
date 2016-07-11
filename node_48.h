//
// Created by philip on 06.07.16.
//

#ifndef ART_NODE64_H
#define ART_NODE64_H


#include <array>
#include <assert.h>
#include "_node.h"
#include "node_16.h"

namespace art
{
    class node_48 : public _node {
    public:
        std::array<uint8_t, 256> child_index;
        std::array<_node *, 48> children{};

        node_48() {
            _count = 0;
            std::fill(child_index.begin(), child_index.end(), EMPTY_MARKER);
        }

        node_48(node_16 *node) {
            assert(node->size() == 16);

            std::fill(child_index.begin(), child_index.end(), EMPTY_MARKER);

            for (uint8_t i = 0; i < 16; i++) {
                child_index[node->keys[i]] = i;
                children[i] = node->children[i];
            }
            _count = 16;

            delete node;
        }

        virtual _node *insert(const Key &key, unsigned depth) override {
            return nullptr;
        }

        virtual void insert(const uint8_t &key_byte, _node *node) override {
            auto pos = _count;
            child_index[key_byte] = pos;
            children[pos] = node;
            _count++;
        }

        virtual _node **find(const uint8_t &key_byte) override {
            if (child_index[key_byte] != EMPTY_MARKER)
                return &children[child_index[key_byte]];
            return nullptr;
        }

        virtual void traverse(unsigned depth) override {
            for (uint8_t i = 0; i < _count; i++)
                children[i]->traverse(depth + 1);
        }

        virtual uint16_t max_size() const override {
            return 48;
        }

        virtual node_type get_type() const override {
            return node_type::node_48_t;
        }
    };
}

#endif //ART_NODE64_H
