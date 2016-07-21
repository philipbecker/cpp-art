#include "catch.hpp"
#include "art/adaptive_radix_tree.h"

using namespace art;

typedef adaptive_radix_tree<int, int>::Key Key;
typedef adaptive_radix_tree<int, int>::_Leaf _leaf;
typedef adaptive_radix_tree<int, int>::_Node_4 node_4;
typedef adaptive_radix_tree<int, int>::_Node_16 node_16;
typedef adaptive_radix_tree<int, int>::_Node_48 node_48;
typedef adaptive_radix_tree<int, int>::_Node_256 node_256;

SCENARIO ("given a node 48 with 16 random elements", "[nodes]") {
    std::vector<int> data(256);
    std::iota(data.begin(), data.end(), 0);
    std::random_shuffle(data.begin(), data.end());

    Key first_key = {data[0]};
    _leaf *first_leaf = new _leaf(std::make_pair(data[0], 0));
    node_4 *node4 = new node_4(first_leaf, first_key.chunks[0]);
    for (int i = 1; i < 4; i++) {
        Key key = {data[i]};
        _leaf *leaf = new _leaf(std::make_pair(data[i], i));
        node4->insert(key.chunks[0], leaf);
    }

    node_16 *node16 = new node_16(node4);
    for (int i = 4; i < 16; i++) {
        Key key = {data[i]};
        _leaf *leaf = new _leaf(std::make_pair(data[i], i));
        node16->insert(key.chunks[0], leaf);
    }

    WHEN ("32 other elements are insereted") {
        node_48 *node48 = new node_48(node16);
        for (int i = 16; i < 48; i++) {
            Key key = {data[i]};
            _leaf *leaf = new _leaf(std::make_pair(data[i], i));
            node48->insert(key.chunks[0], leaf);
        }
        THEN ("the keys are inserted and the children are put at the right position") {
            for (int i = 16; i < 48; i++) {
                REQUIRE(node48->child_index[data[i]] == i);
            }
        }
        THEN ("the node's size is 48") {
            REQUIRE(node48->size() == 48);
        }
    }

}