//
// Created by philip on 06.07.16.
//

#ifndef ART_NODE256_H
#define ART_NODE256_H

#include <bits/unique_ptr.h>
#include "Node.h"
#include "Leaf.h"
#include "Node48.h"

class Node256 : public Node {
public:


    virtual Node **find(const uint8_t &key_byte) override;

    Node256() { }

    Node256(Leaf *pLeaf, unsigned int depth);

    Node256(Node48 *node);

    virtual Node *insert(const Key &key, unsigned depth) override;

    virtual void insert(const uint8_t &key_byte, Node *node) override;

    virtual Node **find(const Key &key, unsigned depth) override;

    virtual void traverse(unsigned depth) override;


private:
    std::array<Node *, 256> children{};
};


#endif //ART_NODE256_H
