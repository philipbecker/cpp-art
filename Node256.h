//
// Created by philip on 06.07.16.
//

#ifndef ART_NODE256_H
#define ART_NODE256_H

#include <bits/unique_ptr.h>
#include "Node.h"
#include "Leaf.h"
#include "Node48.h"

namespace art
{
    class Node256 : public Node {
    public:
        std::array<Node *, 256> children{};

        Node256() { }

        Node256(Leaf *pLeaf, unsigned int depth) {
            Key key = pLeaf->get_key();
            children[key.chunks[depth]] = pLeaf;

            count = 1;
        }

        Node256(Node48 *node) {
            for (uint8_t i = 0; i < 16; i++)
                children[node->child_index[i]] = node->children[node->child_index[i]];
            count = 48;

            delete node;
        }

        virtual Node *insert(const Key &key, unsigned depth) override {
            children[key.chunks[depth]] = new Leaf(key);
            count++;
            return children[key.chunks[depth]];
        }

        virtual void insert(const uint8_t &key_byte, Node *node) override {
            children[key_byte] = node;
            count++;
        }

        virtual Node **find(const uint8_t &key_byte) override {
            return &children[key_byte];
        }

        virtual void traverse(unsigned depth) override {
            std::cout << std::string(depth + 1, '-') << " Node " << this << std::endl;
            for (int i = 0; i < 256; i++) {
                if (children[i] != nullptr) {

                    std::bitset<8> x(i);
                    std::cout << std::string(depth + 1, '-') << " Go along key: " << x << std::endl;

                    children[i]->traverse(depth + 1);
                }
            }
        }

        virtual size_t max_size() const override {
            return 256;
        }

    };
}


#endif //ART_NODE256_H
