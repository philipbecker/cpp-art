//
// Created by philip on 08.07.16.
//
#include "Node4.h"

Node4::Node4(Leaf *leaf, unsigned depth) {
    Key key = leaf->get_key();
    keys[0] = key.chunks[depth];
    children[0] = leaf;

    count = 1;
}

Node *Node4::insert(const Key &key, unsigned depth) {
    return nullptr;
}

void Node4::insert(const uint8_t &key_byte, Node *node) {
    unsigned pos = 0;
    for (; pos < count && keys[pos] < key_byte; pos++);
    std::move(keys.begin() + pos + 1, keys.begin() + count, keys.begin() + pos + 2);
    std::move(children.begin() + pos + 1, children.begin() + count, children.begin() + pos + 2);
    keys[pos] = key_byte;
    children[pos] = node;
    count++;
}

Node **Node4::find(const Key &key, unsigned depth) {
    auto key_byte = key.chunks[depth];
    unsigned pos = 0;
    for (; pos < count && keys[pos] < key_byte; pos++);

    if (keys[pos] == key_byte)
        return &children[pos];
    return nullptr;
}

void Node4::traverse(unsigned depth) {
    for (size_t i = 0; i < count; i++)
        children[i]->traverse(depth + 1);
}

size_t Node4::max_size() const {
    return 4;
}

