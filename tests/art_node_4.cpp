#include "catch.hpp"
#include "../src/Adaptive_radix_tree.h"

using namespace art;

typedef Adaptive_radix_tree<int, int>::Key Key;
typedef Adaptive_radix_tree<int, int>::_Leaf _leaf;
typedef Adaptive_radix_tree<int, int>::_Node_4 node_4;
typedef Adaptive_radix_tree<int, int>::_Node_16 node_16;
typedef Adaptive_radix_tree<int, int>::_Node_48 node_48;
typedef Adaptive_radix_tree<int, int>::_Node_256 node_256;

SCENARIO ("given a node_4 with one element", "[art]") {
    Key key = {261};
    _leaf *leaf_five = new _leaf(key);
    node_4 *node4 = new node_4(leaf_five, 0);

    WHEN ("when 3 elements are inserted in order") {
        std::vector<int> data = {262, 263, 264};
        for (auto &&item : data) {
            Key key2 = {item};
            _leaf *leaf2 = new _leaf(key2);
            node4->insert(key2.chunks[0], leaf2);
        }

        data.push_back(261);
        std::sort(data.begin(), data.end());

        THEN ("its size is equals to 4") {
            REQUIRE(node4->size() == 4);
        }
        THEN ("the keys are stored in sorted order") {
            REQUIRE(std::is_sorted(node4->keys.begin(), node4->keys.end()));
        }
        THEN ("the children are stored at the correct index and contain the right value") {
            for (int i = 0; i < 4; i++) {
                REQUIRE(((_leaf *) node4->children[i])->get_key().chunks[0] == node4->keys[i]);
                REQUIRE(((_leaf *) node4->children[i])->get_key().value == data[i]);
            }
        }
    }

    WHEN ("when 3 elements are inserted in reverse order") {
        std::vector<int> data = {264, 263, 262};
        for (auto &&item : data) {
            Key key2 = {item};
            _leaf *leaf2 = new _leaf(key2);
            node4->insert(key2.chunks[0], leaf2);
        }

        data.push_back(261);
        std::sort(data.begin(), data.end());

        THEN ("its size is equals to 4") {
            REQUIRE(node4->size() == 4);
        }
        THEN ("the keys are stored in sorted order") {
            REQUIRE(std::is_sorted(node4->keys.begin(), node4->keys.end()));
        }
        THEN ("the children are stored at the correct index and contain the right value") {
            for (int i = 0; i < 4; i++) {
                REQUIRE(((_leaf *) node4->children[i])->get_key().chunks[0] == node4->keys[i]);
                REQUIRE(((_leaf *) node4->children[i])->get_key().value == data[i]);
            }
        }
    }

    WHEN ("when 3 elements are inserted in random order") {
        std::vector<int> data = {262, 263, 264};
        std::random_shuffle(data.begin(), data.end());
        for (auto &&item : data) {
            Key key2 = {item};
            _leaf *leaf2 = new _leaf(key2);
            node4->insert(key2.chunks[0], leaf2);
        }

        data.push_back(261);
        std::sort(data.begin(), data.end());

        THEN ("its size is equals to 4") {
            REQUIRE(node4->size() == 4);
        }
        THEN ("the keys are stored in sorted order") {
            REQUIRE(std::is_sorted(node4->keys.begin(), node4->keys.end()));
        }
        THEN ("the children are stored at the correct index and contain the right value") {
            for (int i = 0; i < 4; i++) {
                REQUIRE(((_leaf *) node4->children[i])->get_key().chunks[0] == node4->keys[i]);
                REQUIRE(((_leaf *) node4->children[i])->get_key().value == data[i]);
            }
        }
    }
}
