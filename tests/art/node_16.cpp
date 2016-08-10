#include "catch.hpp"
#include "art/ar_tree.h"
#include "art/radix_map.h"

using namespace art;

typedef std::pair<const int, int> value_type;
typedef ar_tree<int, value_type,
        art::detail::Select1st<value_type>>::Key Key;
typedef ar_tree<int, value_type,
        art::detail::Select1st<value_type>>::_Leaf _leaf;
typedef ar_tree<int, value_type,
        art::detail::Select1st<value_type>>::_Node_4 node_4;
typedef ar_tree<int, value_type,
        art::detail::Select1st<value_type>>::_Node_16 node_16;
typedef ar_tree<int, value_type,
        art::detail::Select1st<value_type>>::_Node_48 node_48;
typedef ar_tree<int, value_type,
        art::detail::Select1st<value_type>>::_Node_256 node_256;

TEST_CASE ("Insert into node 16", "[nodes]") {
    std::vector<int> data = {
            5, 3, 54, 121, 212, 87, 86, 89,
            51, 67, 10, 12, 91, 172, 154, 17
    };
    std::random_shuffle(data.begin(), data.end());


    Key first_key = {data[0]};
    _leaf *first_leaf = new _leaf(std::make_pair(data[0], 0));
    node_4 *node4 = new node_4(first_leaf, first_key.chunks[0], 0);
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
    REQUIRE(std::is_sorted(node16->keys.begin(), node16->keys.end()));
    REQUIRE(node16->size() == 16);


}