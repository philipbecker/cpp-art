//
// Created by philip on 06.07.16.
//

#ifndef ART_NODE64_H
#define ART_NODE64_H


#include <array>
#include <assert.h>
#include "Node.h"
#include "Node16.h"

namespace art
{
    class Node48 : public Node {
    public:
        std::array<uint8_t, 256> child_index;
        std::array<Node *, 48> children{};

        Node48() {
            std::fill(child_index.begin(), child_index.end(), EMPTY_MARKER);
        }

        Node48(Node16 *node) {
            assert(node->size() == 16);

            std::fill(child_index.begin(), child_index.end(), EMPTY_MARKER);

            for (uint8_t i = 0; i < 16; i++) {
                child_index[node->keys[i]] = i;
                children[i] = node->children[i];
            }
            count = 16;

            delete node;
        }

        virtual Node *insert(const Key &key, unsigned depth) override {
            auto key_byte = key.chunks[depth];
            auto pos = this->count;
            if (this->children[pos])
                for (pos = 0; this->children[pos] != nullptr; pos++);
            child_index[key_byte] = pos;
            children[pos] = (Node *) new Leaf(key);
            count++;

            return nullptr;
        }

        virtual void insert(const uint8_t &key_byte, Node *node) override {
            auto pos = this->count;
            child_index[key_byte] = pos;
            children[pos] = node;
            count++;
        }

        virtual Node **find(const uint8_t &key_byte) override {
            if (child_index[key_byte] != EMPTY_MARKER)
                return &children[child_index[key_byte]];
            return nullptr;
        }

        virtual void traverse(unsigned depth) override {
            for (uint8_t i = 0; i < count; i++)
                children[i]->traverse(depth + 1);
        }

        virtual size_t max_size() const override {
            return 48;
        }


    };

}

#endif //ART_NODE64_H
