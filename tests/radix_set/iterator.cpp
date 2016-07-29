#include <map>
#include "catch.hpp"
#include "art/radix_set.h"


SCENARIO("given an empty radix set container", "[radix-set]") {
    art::radix_set<unsigned> set;

    THEN ("iterator's begin is equal to its end") {
        REQUIRE(set.begin() == set.end());
    }
    THEN ("iterator's reverse begin is equal to its reverse end") {
        REQUIRE(set.rbegin() == set.rend());
    }
    WHEN("one element is inserted") {
        set.emplace(5);
        THEN("begin can be dereferenced") {
            REQUIRE(*set.begin() == 5);
        }
        THEN("rbegin can be dereferenced") {
            REQUIRE(*set.rbegin() == 5);
        }
    }
}

SCENARIO("radix set: iteration with unsigned integers compared to std::set", "[radix-set]") {
    art::radix_set<unsigned> radix_set;
    std::set<unsigned> std_set;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned> dis(0, 1000000);

    for (unsigned i = 0; i < 10000; i++) {
        auto k = dis(gen);
        radix_set.emplace(k);
        std_set.emplace(k);
    }

    REQUIRE(*radix_set.begin() == *std_set.begin());
    REQUIRE(*radix_set.rbegin() == *std_set.rbegin());

    THEN ("forward iteration is equal to std::map") {
        auto it_radix = radix_set.begin(), it_radix_end = radix_set.end();
        auto it_std = std_set.begin(), it_std_end = std_set.end();
        for (; it_std != it_std_end; ++it_radix, ++it_std) {
            REQUIRE(*it_radix == *it_std);
        }
        REQUIRE(it_radix == it_radix_end);
    }
    THEN ("reverse iteration is equal to std::map") {
        auto it_radix_reverse = radix_set.rbegin(), it_radix_rend = radix_set.rend();
        auto it_std_reverse = std_set.rbegin(), it_std_rend = std_set.rend();
        for (; it_std_reverse != it_std_rend; ++it_radix_reverse, ++it_std_reverse) {
            REQUIRE(*it_radix_reverse == *it_std_reverse);
        }
        REQUIRE(it_radix_reverse == it_radix_rend);
    }
}

SCENARIO("given a radix set with signed integer key", "[radix-set]") {
    art::radix_set<int> set;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-1000000, 1000000);

    std::vector<int> data;
    for (int i = 0; i < 10000; i++) {
        auto candidate = dis(gen);
        data.push_back(candidate);
    }

    for (auto &d : data)
        set.insert(d);

    THEN("forward iteration is sorted correctly") {
        REQUIRE(std::is_sorted(set.begin(), set.end()));
    }

    THEN("reverse iteration is sorted correctly") {
        REQUIRE(std::is_sorted(set.rbegin(), set.rend(), std::greater<int>()));
    }

}

SCENARIO("given a radix set with unsigned integer key", "[radix-set]") {
    art::radix_set<unsigned> set;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1000000);

    std::vector<unsigned> data;
    for (int i = 0; i < 10000; i++) {
        auto candidate = (unsigned) dis(gen);
        data.push_back(candidate);
    }

    for (auto &d : data)
        set.insert(d);

    THEN("forward iteration is sorted correctly") {
        REQUIRE(std::is_sorted(set.begin(), set.end()));
    }
    THEN("reverse iteration is sorted correctly") {
        REQUIRE(std::is_sorted(set.rbegin(), set.rend(), std::greater<int>()));
    }
}