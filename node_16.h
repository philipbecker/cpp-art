//
// Created by philip on 06.07.16.
//

#ifndef ART_NODE16_H
#define ART_NODE16_H


#include <array>
#include <assert.h>
#include "_node.h"
#include "node_4.h"

namespace art
{
    class node_16 : public _node {
    public:
        std::array<uint8_t, 16> keys{};
        std::array<_node *, 16> children{};

        node_16(node_4 *node) {
            assert(node->size() == 4);

            std::copy(node->keys.begin(), node->keys.end(), keys.begin());
            std::copy(node->children.begin(), node->children.end(), children.begin());
            _count = 4;
            delete node;
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
            for (uint8_t i = 0; i < _count; i++)
                children[i]->traverse(depth + 1);
        }

        virtual uint16_t max_size() const override {
            return 16;
        }

        virtual node_type get_type() const override {
            return node_type::node_16_t;
        }
    };
}


#endif //ART_NODE16_H
