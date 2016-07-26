#include "catch.hpp"
#include "art/adaptive_radix_tree.h"

using namespace art;

typedef adaptive_radix_tree<uint64_t, uint64_t>::Key Key;
typedef adaptive_radix_tree<uint64_t, uint64_t>::_Leaf _leaf;
typedef adaptive_radix_tree<uint64_t, uint64_t>::_Node_4 node_4;
typedef adaptive_radix_tree<uint64_t, uint64_t>::_Node_16 node_16;
typedef adaptive_radix_tree<uint64_t, uint64_t>::_Node_48 node_48;
typedef adaptive_radix_tree<uint64_t, uint64_t>::_Node_256 node_256;

SCENARIO ("given a node_4 with one element", "[nodes]") {
    uint64_t first_value = 261;
    Key key = {first_value};
    _leaf *leaf_five = new _leaf(std::make_pair(first_value, first_value));
    node_4 *node4 = new node_4(leaf_five, key.chunks[0], 0);

    REQUIRE(((_leaf *) node4->children[0])->_value.second == first_value);

    WHEN ("when 3 elements are inserted in order") {
        std::vector<uint64_t> data = {262, 263, 264};
        for (auto &&item : data) {
            Key key2 = {item};
            _leaf *leaf2 = new _leaf(std::make_pair(item, item));
            node4->insert(key2.chunks[0], leaf2);
        }

        data.push_back(first_value);
        std::sort(data.begin(), data.end());

        THEN ("its size is equals to 4") {
            REQUIRE(node4->size() == 4);
        }
        THEN ("the keys are stored in sorted order") {
            REQUIRE(std::is_sorted(node4->keys.begin(), node4->keys.end()));
        }
        THEN ("the children are stored at the correct index and contain the right value") {
            for (int i = 0; i < 4; i++) {
                REQUIRE(((_leaf *) node4->children[0])->_value.second == data[0]);
            }
        }
    }

    WHEN ("when 3 elements are inserted in reverse order") {
        std::vector<uint64_t> data = {264, 263, 262};
        for (auto &&item : data) {
            Key key2 = {item};
            _leaf *leaf2 = new _leaf(std::make_pair(item, item));
            node4->insert(key2.chunks[0], leaf2);
        }

        data.push_back(first_value);
        std::sort(data.begin(), data.end());

        THEN ("its size is equals to 4") {
            REQUIRE(node4->size() == 4);
        }
        THEN ("the keys are stored in sorted order") {
            REQUIRE(std::is_sorted(node4->keys.begin(), node4->keys.end()));
        }
        THEN ("the children are stored at the correct index and contain the right value") {
            for (int i = 0; i < 4; i++) {
                REQUIRE(((_leaf *) node4->children[i])->_value.second == data[i]);
            }
        }
    }

    WHEN ("when 3 elements are inserted in random order") {
        std::vector<uint64_t> data = {262, 263, 264};
        std::random_shuffle(data.begin(), data.end());
        for (auto &&item : data) {
            Key key2 = {item};
            _leaf *leaf2 = new _leaf(std::make_pair(item, item));
            node4->insert(key2.chunks[0], leaf2);
        }

        data.push_back(first_value);
        std::sort(data.begin(), data.end());

        THEN ("its size is equals to 4") {
            REQUIRE(node4->size() == 4);
        }
        THEN ("the keys are stored in sorted order") {
            REQUIRE(std::is_sorted(node4->keys.begin(), node4->keys.end()));
        }
        THEN ("the children are stored at the correct index and contain the right value") {
            for (int i = 0; i < 4; i++) {
                REQUIRE(((_leaf *) node4->children[i])->_value.second == data[i]);
            }
        }
    }
}
