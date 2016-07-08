#include <iostream>
#include <bitset>
#include "Node256.h"
#include "Leaf.h"


Node256::Node256(Leaf *pLeaf, unsigned depth) {
    Key key = pLeaf->get_key();
    children[key.chunks[depth]] = pLeaf;

    count = 1;
}

Node256::Node256(Node48 *node) {
    assert(node->size() == 48);

    for (uint8_t i = 0; i < 16; i++)
        children[node->child_index[i]] = node->children[node->child_index[i]];

    count = 48;
}

Node *Node256::insert(const Key &key, unsigned depth) {
    children[key.chunks[depth]] = new Leaf(key);
    count++;
    return children[key.chunks[depth]];
}

void Node256::insert(const uint8_t &key_byte, Node *node) {
    children[key_byte] = node;
    count++;
}

Node **Node256::find(const Key &key, unsigned depth) {
    return &children[key.chunks[depth]];
}

Node **Node256::find(const uint8_t &key_byte) {
    return &children[key_byte];
}


void Node256::traverse(unsigned depth) {
    std::cout << std::string(depth+1, '-') << " Node " << this << std::endl;
    for (int i = 0; i < 256; i++) {
        if (children[i] != nullptr) {

            std::bitset<8> x(i);
            std::cout << std::string(depth+1, '-') << " Go along key: " << x << std::endl;

            children[i]->traverse(depth+1);
        }
    }
}

