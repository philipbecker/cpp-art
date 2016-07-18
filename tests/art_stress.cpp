#include <map>
#include "catch.hpp"
#include "../src/Adaptive_radix_tree.h"
#include "../src/radix_map.h"


TEST_CASE("Stress tests", "[stress]") {
    art::radix_map<int, int> art;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-1000000, 1000000);

    std::vector<int> data;
    for (int i = 0; i < 100000; i++) {
        auto candidate = dis(gen);
        auto it = std::find(data.begin(), data.end(), candidate);
        if (it == data.end())
            data.push_back(candidate);
    }

    SECTION ("insertion of unqiue elements is successful") {
        const auto size = data.size();
        for (int i = 0; i < size; i++) {
            auto p = art.insert(std::make_pair(data[i], data[i]));
            REQUIRE(p.second);
        }



        SECTION ("can't insert duplicate values") {
            for (int i = 0; i < size; i++) {
                auto p = art.insert(std::make_pair(data[i], data[i]));
                REQUIRE_FALSE(p.second);
            }
        }
    }
}

TEST_CASE("Stress tests 2", "[stress]") {
    std::map<int, int> map;
    art::radix_map<int, int> art;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-100000, 100000);

    for (int i = 0; i < 10000; i++) {
        auto k = dis(gen);
        map.insert(std::make_pair(k, 2*i));
        art.insert(std::make_pair(k, 2*i));
    }

    SECTION ("lower bound") {
        for (int i = -100000; i < 100000; i++) {
            auto it_std = map.lower_bound(i);
            auto it_art = art.lower_bound(i);
            if (it_std != map.end()) {
                REQUIRE((*it_art).first == (*it_std).first);
                REQUIRE((*it_art).second == (*it_std).second);
            } else {
                REQUIRE(it_art == art.end());
            }

            it_std = map.upper_bound(i);
            it_art = art.upper_bound(i);
            if (it_std != map.end()) {
                REQUIRE((*it_art).first == (*it_std).first);
                REQUIRE((*it_art).second == (*it_std).second);
            } else {
                REQUIRE(it_art == art.end());
            }
        }
    }
}