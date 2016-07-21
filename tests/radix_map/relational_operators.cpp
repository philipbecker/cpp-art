#include <map>
#include "catch.hpp"
#include "art/radix_map.h"

TEST_CASE("Relational Operators", "[map-relational-operators]") {
    art::radix_map<int, int> radix_map;
    art::radix_map<int, int> reference;

    SECTION("two empty maps") {
        REQUIRE(radix_map == reference);
        REQUIRE_FALSE(radix_map != reference);
        REQUIRE(radix_map <= reference);
        REQUIRE(radix_map >= reference);
        REQUIRE_FALSE(radix_map < reference);
        REQUIRE_FALSE(radix_map > reference);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-100000, 100000);

    for (int i = 0; i < 10000; i++) {
        auto k = dis(gen);
        radix_map.insert(std::make_pair(k, i));
        reference.insert(std::make_pair(k, i));
    }

    SECTION("two identical maps") {
        REQUIRE(radix_map == reference);
        REQUIRE_FALSE(radix_map != reference);
        REQUIRE(radix_map <= reference);
        REQUIRE(radix_map >= reference);
        REQUIRE_FALSE(radix_map < reference);
        REQUIRE_FALSE(radix_map > reference);
    }

    SECTION("remove one element in the middle") {
        auto it = radix_map.begin();
        for (int i = 0; i < 5000; i++)
            ++it;
        radix_map.erase(it);

        REQUIRE_FALSE(radix_map == reference);
        REQUIRE(radix_map != reference);
        REQUIRE_FALSE(radix_map <= reference);
        REQUIRE(radix_map >= reference);
        REQUIRE_FALSE(radix_map < reference);
        REQUIRE(radix_map > reference);
    }

    SECTION("remove the last element") {
        auto max = --radix_map.end();
        radix_map.erase(max->first);

        REQUIRE_FALSE(radix_map == reference);
        REQUIRE(radix_map != reference);
        REQUIRE(radix_map <= reference);
        REQUIRE_FALSE(radix_map >= reference);
        REQUIRE(radix_map < reference);
        REQUIRE_FALSE(radix_map > reference);
    }
}
