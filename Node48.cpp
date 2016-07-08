//
// Created by philip on 06.07.16.
//

#include "Node48.h"

Node48::Node48() {
    std::fill(child_index.begin(), child_index.end(), EMPTY_MARKER);
}

Node48::Node48(Node16 *pNode) {
    assert(pNode->size() == 16);

    std::fill(child_index.begin(), child_index.end(), EMPTY_MARKER);

    for (uint8_t i = 0; i < 16; i++) {
        child_index[pNode->keys[i]] = i;
        children[i] = pNode->children[i];
    }
}

Node *Node48::insert(const Key &key, unsigned depth) {
    auto key_byte = key.chunks[depth];
    auto pos = this->count;
    if (this->children[pos])
        for (pos = 0; this->children[pos] != nullptr; pos++);
    child_index[key_byte] = pos;
    children[pos] = (Node *) new Leaf(key);
    count++;

    return nullptr;
}

void Node48::insert(const uint8_t &key_byte, Node *node) {
    auto pos = this->count;
    child_index[key_byte] = pos;
    children[pos] = node;
    count++;
}

Node **Node48::find(const Key &key, unsigned depth) {
    return this->find(key.chunks[depth]);
}

Node **Node48::find(const uint8_t &key_byte) {
    if (child_index[key_byte] != EMPTY_MARKER)
        return &children[child_index[key_byte]];
    return nullptr;
}

void Node48::traverse(unsigned depth) {
    for (uint8_t i = 0; i < count; i++)
        children[i]->traverse(depth + 1);
}

size_t Node48::max_size() const {
    return 48;
}
