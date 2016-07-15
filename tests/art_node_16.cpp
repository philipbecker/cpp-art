#include "catch.hpp"
#include "../src/Adaptive_radix_tree.h"

using namespace art;

typedef Adaptive_radix_tree<int, int>::Key Key;
typedef Adaptive_radix_tree<int, int>::_Leaf _leaf;
typedef Adaptive_radix_tree<int, int>::_Node_4 node_4;
typedef Adaptive_radix_tree<int, int>::_Node_16 node_16;
typedef Adaptive_radix_tree<int, int>::_Node_48 node_48;
typedef Adaptive_radix_tree<int, int>::_Node_256 node_256;

TEST_CASE ("Insert into node 16", "[nodes]") {
    std::vector<int> data = {
            5, 3, 54, 121, 212, 87, 86, 89,
            51, 67, 10, 12, 91, 172, 154, 17
    };
    std::random_shuffle(data.begin(), data.end());


    Key first_key = {data[0]};
    _leaf *first_leaf = new _leaf(first_key);
    node_4 *node4 = new node_4(first_leaf, 0);
    for (int i = 1; i < 4; i++) {
        Key key = {data[i]};
        _leaf *leaf = new _leaf(key);
        node4->insert(key.chunks[0], leaf);
    }

    node_16 *node16 = new node_16(node4);

    for (int i = 4; i < 16; i++) {
        Key key = {data[i]};
        _leaf *leaf = new _leaf(key);
        node16->insert(key.chunks[0], leaf);
    }
    REQUIRE(std::is_sorted(node16->keys.begin(), node16->keys.end()));
    REQUIRE(node16->size() == 16);


}