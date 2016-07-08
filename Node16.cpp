#include "Node16.h"

Node16::Node16(Node4 *pNode) {
    assert(pNode->size() == 4);
    std::copy(pNode->keys.begin(), pNode->keys.end(), keys.begin());
    std::copy(pNode->children.begin(), pNode->children.end(), children.begin());

    count = 4;
    delete pNode;
}

Node *Node16::insert(const Key &key, unsigned depth) {
    return nullptr;
}

void Node16::insert(const uint8_t &key_byte, Node *node) {
    unsigned pos = 0;
    for (; pos < count && keys[pos] < key_byte; pos++);
    std::move(keys.begin() + pos + 1, keys.begin() + count, keys.begin() + pos + 2);
    std::move(children.begin() + pos + 1, children.begin() + count, children.begin() + pos + 2);
    keys[pos] = key_byte;
    children[pos] = node;
    count++;
}

Node **Node16::find(const Key &key, unsigned depth) {
    auto key_byte = key.chunks[depth];
    return find(key_byte);
}

Node **Node16::find(const uint8_t &key_byte) {
    unsigned pos = 0;
    for (; pos < count && keys[pos] < key_byte; pos++);

    if (keys[pos] == key_byte)
        return &children[pos];
    return nullptr;
}


void Node16::traverse(unsigned depth) {
    for (uint8_t i = 0; i < count; i++)
        children[i]->traverse(depth + 1);
}
