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

    WHEN("two elements with an equal first key chunk are inserted") {
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

    WHEN("two elements with three equal key chunks are inserted") {
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

