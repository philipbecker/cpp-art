#include <map>
#include "catch.hpp"
#include "art/adaptive_radix_tree.h"
#include "art/radix_map.h"


TEST_CASE("Stress test insert", "[stress]") {
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

TEST_CASE("Stress test emplace", "[stress]") {
    art::radix_map<int, int> art;
    std::map<int, int> std_map;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(-1000000, 1000000);

    std::vector<int> data;
    for (int i = 0; i < 100000; i++) {
        auto candidate = dis(gen);
        auto it = std::find(data.begin(), data.end(), candidate);
        if (it == data.end())
            data.push_back(candidate);
    }

    SECTION ("insertion of unique elements is successful") {
        const auto size = data.size();
        for (int i = 0; i < size; i++) {
            auto p = art.emplace(data[i], i);
            REQUIRE(p.second);
            std_map.emplace(data[i], i);
        }

        SECTION ("can iterate") {
            auto it_radix = art.begin(), it_radix_end = art.end();
            auto it_std = std_map.begin(), it_std_end = std_map.end();
            for (; it_std != it_std_end; ++it_radix, ++it_std) {
                REQUIRE(it_radix->second == it_std->second);
            }
            REQUIRE(it_radix == it_radix_end);
        }

        SECTION ("can't insert duplicate values") {
            for (int i = 0; i < size; i++) {
                auto p = art.emplace(data[i], i);
                REQUIRE_FALSE(p.second);
            }
        }
    }
}

TEST_CASE("Stress test erase by key", "[stress]") {
    art::radix_map<int32_t, int32_t> radix_map;
    std::map<int32_t, int32_t> std_map;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int32_t> dis(-1000000, 1000000);

    std::vector<int32_t> data;
    for (int i = 0; i < 10000; i++) {
        auto candidate = dis(gen);
        auto it = std::find(data.begin(), data.end(), candidate);
        if (it == data.end())
            data.push_back(candidate);
    }

    const auto number_of_elements = data.size();
    for (int i = 0; i < number_of_elements; i++) {
        radix_map.emplace(data[i], i);
        std_map.emplace(data[i], i);
    }

    std::random_shuffle(data.begin(), data.end());

    auto block_size = 50;
    auto rounds = number_of_elements / block_size;
    for (int i = 0; i < rounds; i++) {
        for (int j = 0; j < block_size; j++) {
            radix_map.erase(data[i * block_size + j]);
            std_map.erase(data[i * block_size + j]);
        }

        REQUIRE(radix_map.size() == std_map.size());
        for (int k = (i+1) * block_size; k < data.size(); k++) {
            REQUIRE(radix_map.at(data[k]) == std_map.at(data[k]));
        }

        auto it_radix = radix_map.begin(), it_radix_end = radix_map.end();
        auto it_std = std_map.begin(), it_std_end = std_map.end();
        for (; it_std != it_std_end; ++it_radix, ++it_std) {
            REQUIRE(it_radix->second == it_std->second);
        }
        REQUIRE(it_radix == it_radix_end);
    }
}

TEST_CASE("Stress test erase by iterator", "[stress]") {
    art::radix_map<int32_t, int32_t> radix_map;
    std::map<int32_t, int32_t> std_map;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int32_t> dis(-1000000, 1000000);

    std::vector<int32_t> data;
    for (int i = 0; i < 10000; i++) {
        auto candidate = dis(gen);
        auto it = std::find(data.begin(), data.end(), candidate);
        if (it == data.end())
            data.push_back(candidate);
    }

    const auto number_of_elements = data.size();
    for (int i = 0; i < number_of_elements; i++) {
        radix_map.emplace(data[i], i);
        std_map.emplace(data[i], i);
    }

    std::random_shuffle(data.begin(), data.end());

    auto block_size = 50;
    auto rounds = number_of_elements / block_size;
    for (int i = 0; i < rounds; i++) {
        for (int j = 0; j < block_size; j++) {
            radix_map.erase(radix_map.find(data[i * block_size + j]));
            std_map.erase(data[i * block_size + j]);
        }

        REQUIRE(radix_map.size() == std_map.size());
        for (int k = (i+1) * block_size; k < data.size(); k++) {
            REQUIRE(radix_map.at(data[k]) == std_map.at(data[k]));
        }

        auto it_radix = radix_map.begin(), it_radix_end = radix_map.end();
        auto it_std = std_map.begin(), it_std_end = std_map.end();
        for (; it_std != it_std_end; ++it_radix, ++it_std) {
            REQUIRE(it_radix->second == it_std->second);
        }
        REQUIRE(it_radix == it_radix_end);
    }
}

TEST_CASE("Stress test lower & upper bound", "[stress]") {
    std::map<int, int> map;
    art::radix_map<int, int> art;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-100000, 100000);

    for (int i = 0; i < 10000; i++) {
        auto k = dis(gen);
        map.insert(std::make_pair(k, 2 * i));
        art.insert(std::make_pair(k, 2 * i));
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