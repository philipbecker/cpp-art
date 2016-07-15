#include <map>
#include "catch.hpp"
#include "../src/radix_map.h"


SCENARIO("given an empty container", "[iterator]") {
    art::radix_map<unsigned, unsigned> map;

    THEN ("iterator's begin is equal to its end") {
        REQUIRE(map.begin() == map.end());
    }
    THEN ("iterator's reverse begin is equal to its reverse end") {
        REQUIRE(map.rbegin() == map.rend());
    }
}

SCENARIO("basic iteration", "[iterator]") {
    art::radix_map<unsigned, unsigned> radix_map;
    std::map<unsigned, unsigned> std_map;

    for (unsigned i = 1; i < 10; i++) {
        radix_map.insert(std::pair<unsigned, unsigned>(i, i));
        std_map.emplace(i, i);
    }

    REQUIRE(radix_map.begin()->second == std_map.begin()->second);
    REQUIRE(radix_map.rbegin()->second == std_map.rbegin()->second);
}

SCENARIO("given an art with signed integer key", "[iterator]") {
    art::radix_map<int, int> map;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-1000000, 1000000);

    std::vector<int> data;
    for (int i = 0; i < 10000; i++) {
        auto candidate = dis(gen);
        data.push_back(candidate);
    }

    for (auto &d : data)
        map.insert(std::make_pair(d, d));

    THEN("forward iteration is sorted correctly") {
        REQUIRE(std::is_sorted(map.begin(), map.end()));
    }

    THEN("reverse iteration is sorted correctly") {
        REQUIRE(std::is_sorted(map.rbegin(), map.rend(),
                               [](std::pair<int, int> a, std::pair<int, int> b) {
                                   return std::tie(a.first, a.second) > std::tie(b.first, b.second);
                               })
        );
    }

}

SCENARIO("given an art with unsigned integer key", "[iterator]") {
    art::radix_map<unsigned, unsigned> radix_map;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1000000);

    std::vector<unsigned> data;
    for (int i = 0; i < 10000; i++) {
        auto candidate = (unsigned) dis(gen);
        data.push_back(candidate);
    }

    for (auto &d : data)
        radix_map.insert(std::make_pair(d, d));

    THEN("forward iteration is sorted correctly") {
        REQUIRE(std::is_sorted(radix_map.begin(), radix_map.end()));
    }
    THEN("reverse iteration is sorted correctly") {
        REQUIRE(std::is_sorted(radix_map.rbegin(), radix_map.rend(),
                               [](std::pair<unsigned, unsigned> a, std::pair<unsigned, unsigned> b) {
                                   return std::tie(a.first, a.second) > std::tie(b.first, b.second);
                               })
        );
    }
}