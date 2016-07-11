//
// Created by philip on 06.07.16.
//

#ifndef ART_NODE256_H
#define ART_NODE256_H

#include <bits/unique_ptr.h>
#include "_node.h"
#include "_leaf.h"
#include "node_48.h"

namespace art
{
    class node_256 : public _node {
    public:
        std::array<_node *, 256> children{};

        node_256() { }

        node_256(_leaf *pLeaf, unsigned int depth) {
            Key key = pLeaf->get_key();
            children[key.chunks[depth]] = pLeaf;

            _count = 1;
        }

        node_256(node_48 *node) {
            assert(node->size() == 48);

            for (uint16_t i = 0; i < 256; i++)
                if (node->child_index[i] != EMPTY_MARKER)
                    children[i] = node->children[node->child_index[i]];
            _count = 48;

            delete node;
        }

        virtual _node *insert(const Key &key, unsigned depth) override {
            children[key.chunks[depth]] = new _leaf(key);
            _count++;
            return children[key.chunks[depth]];
        }

        virtual void insert(const uint8_t &key_byte, _node *node) override {
            children[key_byte] = node;
            _count++;
        }

        virtual _node **find(const uint8_t &key_byte) override {
            if (children[key_byte] != nullptr)
                return &children[key_byte];
            return nullptr;
        }

        virtual void traverse(unsigned depth) override {
            std::cout << std::string(depth + 1, '-') << " _node " << this << std::endl;
            for (int i = 0; i < 256; i++) {
                if (children[i] != nullptr) {

                    std::bitset<8> x(i);
                    std::cout << std::string(depth + 1, '-') << " Go along key: " << x << std::endl;

                    children[i]->traverse(depth + 1);
                }
            }
        }

        virtual uint16_t max_size() const override {
            return 256;
        }

        virtual node_type get_type() const override {
            return node_type::node_256_t;
        }

    };
}


#endif //ART_NODE256_H
