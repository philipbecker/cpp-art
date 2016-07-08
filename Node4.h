//
// Created by philip on 08.07.16.
//

#ifndef REFERENCE_NODE4_H
#define REFERENCE_NODE4_H


#include <array>
#include "Node.h"
#include "Leaf.h"

class Node4 : public Node {
public:
    std::array<uint8_t, 4> keys{};
    std::array<Node *, 4> children{};

    virtual size_t max_size() const override;

    Node4(Leaf *leaf, unsigned depth);

    virtual Node *insert(const Key &key, unsigned depth) override;

    virtual void insert(const uint8_t &key_byte, Node *node) override;

    virtual Node **find(const Key &key, unsigned depth) override;

    virtual void traverse(unsigned depth) override;


};


#endif //REFERENCE_NODE4_H
