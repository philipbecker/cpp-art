#include "../libs/catch.h"
#include "../adapt_radix_tree.h"

using namespace art;

TEST_CASE ("Insert into node 16", "[art]") {
    std::vector<int> data = {
            3, 54, 121, 212, 87, 86, 89, 17,
            51, 67, 10, 12, 91, 172, 154
    };

    for (int i = 0; i < 1; i++) {
        Key key = {5};
        Leaf *leaf_five = new Leaf(key);
        Node4 *node4 = new Node4(leaf_five, 0);
        Node16 *node16 = new Node16(node4);

        std::random_shuffle(data.begin(), data.end());
        for (auto &&item : data) {
            Key key2 = {item};
            Leaf *leaf2 = new Leaf(key2);
            node16->insert(key2.chunks[0], leaf2);
        }
        REQUIRE(std::is_sorted(node16->keys.begin(), node16->keys.end()));
        REQUIRE(node16->size() == 16);
    }

}