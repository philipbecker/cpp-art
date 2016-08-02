#include <map>
#include "catch.hpp"
#include "art/radix_set.h"


TEST_CASE("Unsigned 8 bit keys are supported", "[art]") {
    art::radix_set<uint8_t> set;

    auto p = set.insert(uint8_t(255));
    REQUIRE(p.second);
}

TEST_CASE("Signed 8 bit keys are supported", "[art]") {
    art::radix_set<int8_t> set;

    auto p = set.insert(uint8_t(127));
    REQUIRE(p.second);
    p = set.insert(uint8_t(-128));
    REQUIRE(p.second);
}

TEST_CASE("Unsigned 16 bit keys are supported", "[art]") {
    art::radix_set<uint16_t> set;

    auto p = set.insert(uint16_t(255));
    REQUIRE(p.second);
}

TEST_CASE("Signed 16 bit keys are supported", "[art]") {
    art::radix_set<int16_t> set;

    auto plus_one = set.insert(int16_t(1));
    REQUIRE(plus_one.second);
    auto zero = set.insert(int16_t(0));
    REQUIRE(zero.second);
    auto minus_one = set.insert(int16_t(-1));
    REQUIRE(minus_one.second);

    REQUIRE(std::distance(minus_one.first, plus_one.first) == 2);
}

TEST_CASE("Unsigned 32 bit keys are supported", "[art]") {
    art::radix_set<uint32_t> set;

    auto p = set.insert(uint32_t(255));
    REQUIRE(p.second);
}

TEST_CASE("Signed 32 bit keys are supported", "[art]") {
    art::radix_set<int32_t> set;

    auto plus_one = set.insert(int32_t(1));
    REQUIRE(plus_one.second);
    auto zero = set.insert(int32_t(0));
    REQUIRE(zero.second);
    auto minus_one = set.insert(int32_t(-1));
    REQUIRE(minus_one.second);

    REQUIRE(std::distance(minus_one.first, plus_one.first) == 2);
}


TEST_CASE("Unsigned 64 bit keys are supported", "[art]") {
    art::radix_set<uint64_t> set;

    auto p = set.insert(uint64_t(255));
    REQUIRE(p.second);
}

TEST_CASE("Signed 64 bit keys are supported", "[art]") {
    art::radix_set<int64_t> set;

    auto plus_one = set.insert(int64_t(1));
    REQUIRE(plus_one.second);
    auto zero = set.insert(int64_t(0));
    REQUIRE(zero.second);
    auto minus_one = set.insert(int64_t(-1));
    REQUIRE(minus_one.second);

    REQUIRE(std::distance(minus_one.first, plus_one.first) == 2);
}

TEST_CASE("Pairs of supported keys are supported", "[art]") {
    art::radix_set<std::pair<short, int>> set;

    auto zero_five = set.insert(std::make_pair(0, 5));
    REQUIRE(zero_five.second);
    auto zero_six = set.insert(std::make_pair(0, 6));
    REQUIRE(zero_six.second);
    auto zero_four = set.insert(std::make_pair(0, 4));
    REQUIRE(zero_four.second);
    auto minus_one = set.insert(std::make_pair(-1, 400));
    REQUIRE(minus_one.second);
    auto plus_one = set.insert(std::make_pair(1, -300));
    REQUIRE(plus_one.second);

    std::vector<std::pair<short, long> > v = {
            *minus_one.first, *zero_four.first, *zero_five.first,
            *zero_six.first, *plus_one.first
    };

    REQUIRE(set.size() == v.size());
    // REQUIRE(std::equal(v.begin(), v.end(), set.begin()));
}