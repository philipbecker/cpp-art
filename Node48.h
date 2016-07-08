//
// Created by philip on 06.07.16.
//

#ifndef ART_NODE64_H
#define ART_NODE64_H


#include <array>
#include <assert.h>
#include "Node.h"
#include "Node16.h"

class Node48 : public Node {
public:
    Node48();

    Node48(Node16 *pNode);

    virtual Node *insert(const Key &key, unsigned depth) override;

    virtual void insert(const uint8_t &key_byte, Node *node) override;

    virtual Node **find(const Key &key, unsigned depth) override;

    virtual Node **find(const uint8_t &key_byte) override;

    virtual void traverse(unsigned depth) override;

    std::array<uint8_t, 256> child_index;
    std::array<Node *, 48> children{};
};


#endif //ART_NODE64_H
