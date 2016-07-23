#include "catch.hpp"
#include "art/adaptive_radix_tree.h"


TEST_CASE("Can tiebreak at level 1", "[art]") {
    art::adaptive_radix_tree<int, int> art;
    art.insert_unique(std::make_pair(5, 5));
    art.insert_unique(std::make_pair(6, 6));
    art.insert_unique(std::make_pair(261, 261));
    REQUIRE(art.find(5) != art.end());
    REQUIRE(art.find(6) != art.end());
    REQUIRE(art.find(261) != art.end());
}

SCENARIO("growing the root node", "[art]") {
    art::adaptive_radix_tree<uint64_t, uint64_t> art;

    std::vector<uint64_t> data(256);
    std::iota(data.begin(), data.end(), 0);
    // The first key byte needs to be decisive,
    // otherwise the root won't grow as desired
    for (auto &d : data)
        d <<= 56;

    WHEN("first 5 values are inserted") {
        for (uint64_t i = 0; i < 5; i++) {
            art.insert_unique(std::make_pair(data[i], data[i]));
        }

        THEN("root grows to node 16") {
            REQUIRE(art._M_root->size() == 5);
            REQUIRE(art._M_root->get_type() == 2);
        }

        AND_WHEN("12 more values are inserted") {
            for (uint64_t i = 5; i < 17; i++) {
                art.insert_unique(std::make_pair(data[i], data[i]));
            }

            THEN ("root has grown to node 48") {
                REQUIRE(art._M_root->size() == 17);
                REQUIRE(art._M_root->get_type() == 3);
            };
            AND_WHEN("32 more values are inserted") {
                for (uint64_t i = 17; i < 49; i++) {
                    art.insert_unique(std::make_pair(data[i], data[i]));
                }

                THEN ("root has grown to node 256") {
                    REQUIRE(art._M_root->size() == 49);
                    REQUIRE(art._M_root->get_type() == 4);
                };
            }
        }
    }
}
