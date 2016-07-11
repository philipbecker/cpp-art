//
// Created by philip on 06.07.16.
//

#ifndef ART_NODE16_H
#define ART_NODE16_H


#include <array>
#include <assert.h>
#include "Node.h"
#include "node_4.h"

namespace art
{
    class node_16 : public Node {
    public:
        std::array<Node *, 16> children{};
        std::array<uint8_t, 16> keys{};

        node_16(node_4 *node) {
            std::copy(node->keys.begin(), node->keys.end(), keys.begin());
            std::copy(node->children.begin(), node->children.end(), children.begin());

            _count = node->size();
            delete node;
        }

        virtual Node *insert(const Key &key, unsigned depth) override {
            return nullptr;
        }

        virtual void insert(const uint8_t &key_byte, Node *node) override {
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

        virtual Node **find(const uint8_t &key_byte) override {
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

        virtual size_t max_size() const override {
            return 16;
        }

        virtual node_type get_type() const override {
            return node_type::node_16;
        }
    };
}


#endif //ART_NODE16_H
