#include <map>
#include "catch.hpp"
#include "art/radix_map.h"

TEST_CASE("Value compare of signed integers", "[map-value-compare]") {
    art::radix_map<int, int> radix_map;

    auto min = std::pair<int, int>(std::numeric_limits<int>::min(), 200);
    auto negative = std::pair<int, int>(-1, 100);
    auto zero = std::pair<int, int>(0, 100);
    auto positive = std::pair<int, int>(1, 100);
    auto max = std::pair<int, int>(std::numeric_limits<int>::max(), -200);

    REQUIRE(radix_map.value_comp()(negative, positive));
    REQUIRE_FALSE(radix_map.value_comp()(positive, negative));
    REQUIRE(radix_map.value_comp()(negative, zero));
    REQUIRE_FALSE(radix_map.value_comp()(zero, negative));
    REQUIRE(radix_map.value_comp()(zero, positive));
    REQUIRE_FALSE(radix_map.value_comp()(positive, zero));

    SECTION ("comparison with max value") {
        REQUIRE(radix_map.value_comp()(min, max));
        REQUIRE_FALSE(radix_map.value_comp()(max, min));
        REQUIRE(radix_map.value_comp()(negative, max));
        REQUIRE_FALSE(radix_map.value_comp()(max, negative));
        REQUIRE(radix_map.value_comp()(positive, max));
        REQUIRE_FALSE(radix_map.value_comp()(max, positive));
        REQUIRE(radix_map.value_comp()(zero, max));
        REQUIRE_FALSE(radix_map.value_comp()(max, zero));
    }

    SECTION ("comparison with min value") {
        REQUIRE(radix_map.value_comp()(min, negative));
        REQUIRE_FALSE(radix_map.value_comp()(negative, min));
        REQUIRE(radix_map.value_comp()(min, positive));
        REQUIRE_FALSE(radix_map.value_comp()(positive, min));
        REQUIRE(radix_map.value_comp()(min, zero));
        REQUIRE_FALSE(radix_map.value_comp()(zero, min));
    }

    SECTION("compare equal") {
        REQUIRE_FALSE(radix_map.value_comp()(min, min));
        REQUIRE_FALSE(radix_map.value_comp()(negative, negative));
        REQUIRE_FALSE(radix_map.value_comp()(zero, zero));
        REQUIRE_FALSE(radix_map.value_comp()(positive, positive));
        REQUIRE_FALSE(radix_map.value_comp()(max, max));
    }
}

