//
// Created by philip on 06.07.16.
//

#ifndef ART_NODE16_H
#define ART_NODE16_H


#include <array>
#include <assert.h>
#include "Node.h"
#include "Node4.h"

class Node16 : public Node {
public:
    virtual Node **find(const uint8_t &key_byte) override;

    Node16(Node4 *pNode);

    virtual Node *insert(const Key &key, unsigned depth) override;

    virtual void insert(const uint8_t &key_byte, Node *node) override;

    virtual Node **find(const Key &key, unsigned depth) override;

    virtual void traverse(unsigned depth) override;

    std::array<Node *, 16> children{};
    std::array<uint8_t, 16> keys{};
};


#endif //ART_NODE16_H
