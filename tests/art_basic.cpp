#include "../libs/catch.h"
#include "../adapt_radix_tree.h"


SCENARIO("given an empty art", "[art]") {
    art::adapt_radix_tree<int, int> art;

    THEN("its size is 0") {
        REQUIRE(art.size() == 0);
    }
    WHEN("an element is inserted") {
        art.insert(std::make_pair(5, 5));
        THEN("its size is 1 and the inserted value is correct") {
            REQUIRE(art.size() == 1);
            REQUIRE(art.find(5));
        }

        AND_WHEN("the same element is inserted again") {
            auto p = art.insert(std::make_pair(5, 5));
            THEN("the insertion failed and its size is still 1") {
                REQUIRE_FALSE(p.second);
                REQUIRE(art.size() == 1);
                REQUIRE(art.find(5));
            }
        }
    }

    WHEN("two elements with an equal first prefix chunk are inserted") {
        auto p1 = art.insert(std::make_pair(5, 5));
        auto p2 = art.insert(std::make_pair(261, 261));
        THEN("both elements were inserted successfully") {
            REQUIRE(p1.second);
            REQUIRE(p2.second);
            REQUIRE(art.find(5));
            REQUIRE(art.find(261));
            REQUIRE(art.size() == 2);
        }
    }

    WHEN("two elements with three equal prefix chunks are inserted") {
        auto p1 = art.insert(std::make_pair(5, 5));
        auto p2 = art.insert(std::make_pair(16777221, 16777221));
        THEN("both elements were inserted successfully") {
            REQUIRE(p1.second);
            REQUIRE(p2.second);
            REQUIRE(art.find(5));
            REQUIRE(art.find(16777221));
            REQUIRE(art.size() == 2);
        }
    }
}

TEST_CASE("Can tiebreak at level 1", "[art]") {
    art::adapt_radix_tree<int, int> art;
    art.insert(std::make_pair(5, 5));
    art.insert(std::make_pair(6, 6));
    art.insert(std::make_pair(261, 261));
    REQUIRE(art.find(5));
    REQUIRE(art.find(6));
    REQUIRE(art.find(261));
}


SCENARIO("growing the root node", "[art]") {
    art::adapt_radix_tree<uint64_t , uint64_t> art;

    std::vector<uint64_t> data(256);
    std::iota(data.begin(), data.end(), 0);
    // The first key byte needs to be decisive,
    // otherwise the root won't grow as desired
    for (auto& d : data)
        d <<= 56;

    WHEN("first 5 values are inserted") {
        for (uint64_t i = 0; i < 5; i++) {
            art.insert(std::make_pair(data[i], data[i]));
        }

        THEN("root grows to node 16") {
            REQUIRE(art._root->size() == 5);
            REQUIRE(art._root->get_type() == 2);
        }

        AND_WHEN("12 more values are inserted") {
            for (uint64_t i = 5; i < 17; i++) {
                art.insert(std::make_pair(data[i], data[i]));
            }

            THEN ("root has grown to node 48") {
                    REQUIRE(art._root->size() == 17);
                    REQUIRE(art._root->get_type() == 3);
            };
            AND_WHEN("32 more values are inserted") {
                for (uint64_t i = 17; i < 49; i++) {
                    art.insert(std::make_pair(data[i], data[i]));
                }

                THEN ("root has grown to node 256") {
                    REQUIRE(art._root->size() == 49);
                    REQUIRE(art._root->get_type() == 4);
                };
            }
        }
    }
}

