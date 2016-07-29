#include <map>
#include "catch.hpp"
#include "art/radix_map.h"


TEST_CASE("Maximum size of 1 byte key is 2^8", "[radix-map]") {
    art::radix_map<int8_t, std::string> map;
    REQUIRE(map.max_size() == 256);
}

TEST_CASE("Maximum size of 2 byte key is 2^16", "[radix-map]") {
    art::radix_map<int16_t, std::string> map;
    REQUIRE(map.max_size() == 65536);
}

TEST_CASE("Maximum size of 4 byte key is 2^32", "[radix-map]") {
    art::radix_map<int32_t, std::string> map;
    REQUIRE(map.max_size() == 4294967296);
}

TEST_CASE("Maximum size of 8 byte key is 2^64", "[radix-map]") {
    art::radix_map<int64_t, std::string> map;
    REQUIRE(map.max_size() == std::numeric_limits<size_t>::max());
}

