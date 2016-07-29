#include <map>
#include "catch.hpp"
#include "art/radix_map.h"

SCENARIO("given an empty art", "[radix-map]") {
    art::radix_map<int32_t, int32_t> art;

    THEN("its size is 0") {
        REQUIRE(art.size() == 0);
    }
    THEN("its empty") {
        REQUIRE(art.empty());
    }
    WHEN("an element is inserted") {
        art.insert(std::make_pair(5, 27));
        THEN("its size is 1 and the inserted value is correct") {
            REQUIRE(art.size() == 1);
            REQUIRE_FALSE(art.empty());
            REQUIRE(art.find(5) != art.end());
            REQUIRE(art.find(5)->second == 27);
        }

        AND_WHEN("the same element is inserted again") {
            auto p = art.insert(std::make_pair(5, 27));
            THEN("the insertion failed and its size is still 1") {
                REQUIRE_FALSE(p.second);
                REQUIRE(art.size() == 1);
                REQUIRE(art.find(5) != art.end());
                REQUIRE(art.find(5)->second == 27);
            }
        }
    }
}

TEST_CASE("basic operations", "[radix-map]") {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(std::numeric_limits<int>::min(),
                                        std::numeric_limits<int>::max());

    std::vector<std::pair<int, int> > v;
    for (int i = 0; i < 10000; i++) {
        auto candidate = dis(gen);
        auto it = std::find(v.begin(), v.end(), std::make_pair(candidate, 3 * candidate));
        if (it == v.end())
            v.push_back(std::make_pair(candidate, 3 * candidate));
    }

    SECTION ("range constructor") {
        art::radix_map<int, int> art(v.begin(), v.end());

        REQUIRE(art.size() == v.size());

        for (int i = 0; i < v.size(); i++)
            REQUIRE((*art.find(v[i].first)).second == v[i].second);
    }

    SECTION ("range insertion") {
        art::radix_map<int, int> art;
        art.insert(v.begin(), v.end());

        REQUIRE(art.size() == v.size());

        for (int i = 0; i < v.size(); i++)
            REQUIRE((*art.find(v[i].first)).second == v[i].second);
    }

    SECTION ("construct with initializer list") {
        art::radix_map<int, int> art;
        art.insert({{1,   3},
                    {2,   12},
                    {-20, 1512}}
        );

        REQUIRE(art.size() == 3);
        REQUIRE((*art.find(1)).second == 3);
        REQUIRE((*art.find(2)).second == 12);
        REQUIRE((*art.find(-20)).second == 1512);

        SECTION ("assignment with initializer list") {
            art = {{42,   721},
                   {321,  521},
                   {-51,  21},
                   {-502, 22}};
            REQUIRE(art.size() == 4);
            REQUIRE(art.find(1) == art.end());
            REQUIRE(art.find(2) == art.end());
            REQUIRE(art.find(-20) == art.end());
            REQUIRE((*art.find(42)).second == 721);
            REQUIRE((*art.find(321)).second == 521);
            REQUIRE((*art.find(-51)).second == 21);
            REQUIRE((*art.find(-502)).second == 22);
        }
    }

    SECTION ("lookup operations") {
        art::radix_map<int, int> art(v.begin(), v.end());

        REQUIRE(art.size() == v.size());

        SECTION ("map.at") {
            for (int i = 0; i < v.size(); i++) {
                REQUIRE(art.at(v[i].first) == v[i].second);
            }
        }

        SECTION ("map.count(key)") {
            for (int i = 0; i < v.size(); i++) {
                REQUIRE(art.count(v[i].first) == 1);
            }
        }
    }
}

TEST_CASE("Erase all elements", "[radix-map]") {
    std::map<unsigned, unsigned> map;
    art::radix_map<unsigned, unsigned> radix_map;

    for (unsigned i = 0; i < 10000; i++) {
        map.insert(std::make_pair(i, 2 * i));
        radix_map.insert(std::make_pair(i, 2 * i));
    }

    SECTION ("erase equals std::map") {
        for (unsigned i = 0; i < 10000; i++) {
            auto std_res = map.erase(i);
            auto art_res = radix_map.erase(i);
            REQUIRE(std_res == art_res);
            REQUIRE(map.size() == radix_map.size());
        }
    }
}

TEST_CASE("Erase all elements and keys not in map", "[radix-map]") {
    std::map<int, int> map;
    art::radix_map<int, int> radix_map;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-100000, 100000);

    for (int i = 0; i < 10000; i++) {
        auto k = dis(gen);
        map.insert(std::make_pair(k, 2 * i));
        radix_map.insert(std::make_pair(k, 2 * i));
    }

    SECTION ("erase equals std::map") {
        for (int i = -100000; i < 100000; i++) {
            auto std_res = map.erase(i);
            auto art_res = radix_map.erase(i);
            REQUIRE(std_res == art_res);
            REQUIRE(map.size() == radix_map.size());
        }
    }
}


TEST_CASE("Erase with iterators", "[radix-map]") {
    std::map<int, int> map;
    art::radix_map<int, int> radix_map;

    for (int i = 0; i < 10000; i++) {
        map.insert(std::make_pair(i, 2 * i));
        radix_map.insert(std::make_pair(i, 2 * i));
    }

    SECTION ("erase iterators from map") {
        auto map_next = map.erase(map.find(54));
        auto radix_next = radix_map.erase(radix_map.find(54));
        REQUIRE(radix_next->first == map_next->first);
        REQUIRE(map.size() == radix_map.size());
        REQUIRE(map.find(54) == map.end());
        REQUIRE(radix_map.find(54) == radix_map.end());
    }

    SECTION ("erase minimum key by iterator") {
        auto map_next = map.erase(map.find(0));
        auto radix_next = radix_map.erase(radix_map.find(0));
        REQUIRE(radix_next->first == map_next->first);
        REQUIRE(map.size() == radix_map.size());
        REQUIRE(map.find(0) == map.end());
        REQUIRE(radix_map.find(0) == radix_map.end());
    }

    SECTION ("erase maximum key by iterator") {
        auto map_next = map.erase(map.find(9999));
        auto radix_next = radix_map.erase(radix_map.find(9999));
        REQUIRE(radix_next == radix_map.end());
        REQUIRE(map_next == map.end());
        REQUIRE(map.size() == radix_map.size());
        REQUIRE(map.find(9999) == map.end());
        REQUIRE(radix_map.find(9999) == radix_map.end());
    }

    SECTION ("Erase ranges from map") {
        map.erase(map.find(54), map.find(242));
        radix_map.erase(radix_map.find(54), radix_map.find(242));
        REQUIRE(map.size() == radix_map.size());
        REQUIRE(map.find(54) == map.end());
        REQUIRE(map.find(241) == map.end());
        REQUIRE(radix_map.find(54) == radix_map.end());
        REQUIRE(radix_map.find(241) == radix_map.end());
    }
}

TEST_CASE("Operator[]", "[radix-map]") {
    std::map<int, int> map;
    art::radix_map<int, int> art;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-100000, 100000);

    for (int i = 0; i < 20000; i++) {
        auto k = dis(gen);
        map.insert(std::make_pair(k, 2 * i));
        art.insert(std::make_pair(k, 2 * i));
    }

    for (int i = -100000; i <= 100000; i++) {
        auto k = dis(gen);
        if (map.find(k) != map.end()) {
            // Overwrite existing value
            map[k] = i;
            art[k] = i;
            REQUIRE(map[k] == art[k]);
            REQUIRE((*art.find(k)).first == (*map.find(k)).first);
            REQUIRE((*art.find(k)).second == (*map.find(k)).second);
        } else {
            // Insert new value
            map[k] = i;
            art[k] = i;
            REQUIRE(map[k] == art[k]);
            REQUIRE((*art.find(k)).first == (*map.find(k)).first);
            REQUIRE((*art.find(k)).second == (*map.find(k)).second);
        }
    }
}

TEST_CASE("Operator at", "[radix-map]") {
    std::map<int, int> map;
    art::radix_map<int, int> art;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-100000, 100000);

    for (int i = 0; i < 20000; i++) {
        auto k = dis(gen);
        map.insert(std::make_pair(k, 2 * i));
        art.insert(std::make_pair(k, 2 * i));
    }

    for (int i = -100000; i <= 100000; i++) {
        auto k = dis(gen);
        if (map.find(k) != map.end()) {
            // Overwrite existing value
            map.at(k) = i;
            art.at(k) = i;
            REQUIRE(art.at(k) == map.at(k));
            REQUIRE((*art.find(k)).first == (*map.find(k)).first);
            REQUIRE((*art.find(k)).second == (*map.find(k)).second);
        } else {
            // Key not in map, expect out of range exception
            REQUIRE_THROWS(art.at(k));
        }
    }
}

TEST_CASE("lower_bound", "[radix-map]") {
    art::radix_map<int, int> art;

    art.emplace(-5, -5);
    art.emplace(5, 5);

    REQUIRE(art.lower_bound(-255)->first == -5);
    REQUIRE(art.lower_bound(-6)->first == -5);
    REQUIRE(art.lower_bound(-5)->first == -5);
    REQUIRE(art.lower_bound(-4)->first == 5);
    REQUIRE(art.lower_bound(0)->first == 5);
    REQUIRE(art.lower_bound(5)->first == 5);
}

TEST_CASE("upper_bound", "[radix-map]") {
    art::radix_map<int, int> art;

    art.emplace(-5, -5);
    art.emplace(5, 5);

    REQUIRE(art.upper_bound(-255)->first == -5);
    REQUIRE(art.upper_bound(-6)->first == -5);
    REQUIRE(art.upper_bound(-5)->first == 5);
    REQUIRE(art.upper_bound(-4)->first == 5);
    REQUIRE(art.upper_bound(0)->first == 5);
    REQUIRE(art.upper_bound(5) == art.end());
}