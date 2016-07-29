#include <map>
#include "catch.hpp"
#include "art/radix_set.h"

SCENARIO("given an empty radix set", "[radix-set]") {
    art::radix_set<int32_t> set;

    THEN("its size is 0") {
        REQUIRE(set.size() == 0);
    }
    THEN("its empty") {
        REQUIRE(set.empty());
    }
    WHEN("an element is inserted") {
        set.insert(5);
        THEN("its size is 1 and the inserted value is correct") {
            REQUIRE(set.size() == 1);
            REQUIRE_FALSE(set.empty());
            REQUIRE(set.find(5) != set.end());
            REQUIRE(*set.find(5) == 5);
        }

        AND_WHEN("the same element is inserted again") {
            auto p = set.insert(5);
            THEN("the insertion failed and its size is still 1") {
                REQUIRE_FALSE(p.second);
                REQUIRE(set.size() == 1);
                REQUIRE(set.find(5) != set.end());
                REQUIRE(*set.find(5) == 5);
            }
        }
    }
}

TEST_CASE("radix set basic operations", "[radix-set]") {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(std::numeric_limits<int>::min(),
                                        std::numeric_limits<int>::max());

    std::vector<int> v;
    for (int i = 0; i < 10000; i++) {
        auto candidate = dis(gen);
        auto it = std::find(v.begin(), v.end(), candidate);
        if (it == v.end())
            v.push_back(candidate);
    }

    SECTION ("range constructor") {
        art::radix_set<int> set(v.begin(), v.end());

        REQUIRE(set.size() == v.size());

        for (int i = 0; i < v.size(); i++)
            REQUIRE(*set.find(v[i]) == v[i]);
    }

    SECTION ("range insertion") {
        art::radix_set<int> set;
        set.insert(v.begin(), v.end());

        REQUIRE(set.size() == v.size());

        for (int i = 0; i < v.size(); i++)
            REQUIRE(*set.find(v[i]) == v[i]);
    }

    SECTION ("construct with initializer list") {
        art::radix_set<int> set;
        set.insert({1, -20, 2});

        REQUIRE(set.size() == 3);
        REQUIRE(*set.find(1) == 1);
        REQUIRE(*set.find(2) == 2);
        REQUIRE(*set.find(-20) == -20);

        SECTION ("assignment with initializer list") {
            set = {42, 321, -51, -502};
            REQUIRE(set.size() == 4);
            REQUIRE(set.find(1) == set.end());
            REQUIRE(set.find(2) == set.end());
            REQUIRE(set.find(-20) == set.end());
            REQUIRE(*set.find(42) == 42);
            REQUIRE(*set.find(321) == 321);
            REQUIRE(*set.find(-51) == -51);
            REQUIRE(*set.find(-502) == -502);
        }
    }

    SECTION ("lookup operations") {
        art::radix_set<int> set(v.begin(), v.end());

        REQUIRE(set.size() == v.size());

        SECTION ("map.at") {
            for (int i = 0; i < v.size(); i++) {
                REQUIRE(*set.find(v[i]) == v[i]);
            }
        }

        SECTION ("map.count(key)") {
            for (int i = 0; i < v.size(); i++) {
                REQUIRE(set.count(v[i]) == 1);
            }
        }
    }
}

TEST_CASE("Erase all elements in radix set", "[radix-set]") {
    std::set<unsigned> set;
    art::radix_set<unsigned> radix_set;

    for (unsigned i = 0; i < 10000; i++) {
        set.insert(i);
        radix_set.insert(i);
    }

    SECTION ("erase equals std::map") {
        for (unsigned i = 0; i < 10000; i++) {
            auto std_res = set.erase(i);
            auto art_res = radix_set.erase(i);
            REQUIRE(std_res == art_res);
            REQUIRE(set.size() == radix_set.size());
        }
    }
}

TEST_CASE("Erase all elements not in radix set", "[radix-set]") {
    std::set<int> set;
    art::radix_set<int> radix_set;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-100000, 100000);

    for (int i = 0; i < 10000; i++) {
        auto k = dis(gen);
        set.insert(k);
        radix_set.insert(k);
    }

    SECTION ("erase equals std::map") {
        for (int i = -100000; i < 100000; i++) {
            auto std_res = set.erase(i);
            auto art_res = radix_set.erase(i);
            REQUIRE(std_res == art_res);
            REQUIRE(set.size() == radix_set.size());
        }
    }
}


TEST_CASE("Erase with iterators in radix set", "[radix-set]") {
    std::set<int> set;
    art::radix_set<int> radix_set;

    for (int i = 0; i < 10000; i++) {
        set.insert(i);
        radix_set.insert(i);
    }

    SECTION ("erase iterators from map") {
        auto set_next = set.erase(set.find(54));
        auto radix_next = radix_set.erase(radix_set.find(54));
        REQUIRE(*radix_next == *set_next);
        REQUIRE(set.size() == radix_set.size());
        REQUIRE(set.find(54) == set.end());
        REQUIRE(radix_set.find(54) == radix_set.end());
    }

    SECTION ("erase minimum key by iterator") {
        auto set_next = set.erase(set.find(0));
        auto radix_next = radix_set.erase(radix_set.find(0));
        REQUIRE(*radix_next == *set_next);
        REQUIRE(set.size() == radix_set.size());
        REQUIRE(set.find(0) == set.end());
        REQUIRE(radix_set.find(0) == radix_set.end());
    }

    SECTION ("erase maximum key by iterator") {
        auto set_next = set.erase(set.find(9999));
        auto radix_next = radix_set.erase(radix_set.find(9999));
        REQUIRE(radix_next == radix_set.end());
        REQUIRE(set_next == set.end());
        REQUIRE(set.size() == radix_set.size());
        REQUIRE(set.find(9999) == set.end());
        REQUIRE(radix_set.find(9999) == radix_set.end());
    }

    SECTION ("Erase ranges from map") {
        set.erase(set.find(54), set.find(242));
        radix_set.erase(radix_set.find(54), radix_set.find(242));
        REQUIRE(set.size() == radix_set.size());
        REQUIRE(set.find(54) == set.end());
        REQUIRE(set.find(241) == set.end());
        REQUIRE(radix_set.find(54) == radix_set.end());
        REQUIRE(radix_set.find(241) == radix_set.end());
    }
}


TEST_CASE("lower_bound of radix set", "[radix-set]") {
    art::radix_set<int> radix_set;

    radix_set.emplace(-5);
    radix_set.emplace(5);

    REQUIRE(*radix_set.lower_bound(-255) == -5);
    REQUIRE(*radix_set.lower_bound(-6) == -5);
    REQUIRE(*radix_set.lower_bound(-5) == -5);
    REQUIRE(*radix_set.lower_bound(-4) == 5);
    REQUIRE(*radix_set.lower_bound(0) == 5);
    REQUIRE(*radix_set.lower_bound(5) == 5);
    REQUIRE(radix_set.lower_bound(6) == radix_set.end());
}

TEST_CASE("upper_bound of radix set", "[radix-set]") {
    art::radix_set<int> radix_set;

    radix_set.emplace(-5);
    radix_set.emplace(5);

    REQUIRE(*radix_set.upper_bound(-255) == -5);
    REQUIRE(*radix_set.upper_bound(-6) == -5);
    REQUIRE(*radix_set.upper_bound(-5) == 5);
    REQUIRE(*radix_set.upper_bound(-4) == 5);
    REQUIRE(*radix_set.upper_bound(0) == 5);
    REQUIRE(*radix_set.upper_bound(4) == 5);
    REQUIRE(radix_set.upper_bound(5) == radix_set.end());
}