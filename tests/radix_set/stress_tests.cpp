#include <map>
#include "catch.hpp"
#include "art/radix_set.h"


TEST_CASE("Radix set stress test insert", "[radix-set-stress]") {
    art::radix_set<int> set;

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

    SECTION ("insertion of unique elements is successful") {
        const auto size = data.size();
        for (int i = 0; i < size; i++) {
            auto p = set.insert(data[i]);
            REQUIRE(p.second);
        }

        SECTION ("can't insert duplicate values") {
            for (int i = 0; i < size; i++) {
                auto p = set.insert(data[i]);
                REQUIRE_FALSE(p.second);
            }
        }
    }
}

TEST_CASE("Radix set stress test emplace", "[radix-set-stress]") {
    art::radix_set<int> radix_set;
    std::set<int> std_set;

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
            auto p = radix_set.emplace(data[i]);
            REQUIRE(p.second);
            std_set.emplace(data[i]);
        }

        SECTION ("can iterate") {
            auto it_radix = radix_set.begin(), it_radix_end = radix_set.end();
            auto it_std = std_set.begin(), it_std_end = std_set.end();
            for (; it_std != it_std_end; ++it_radix, ++it_std) {
                REQUIRE(*it_radix == *it_std);
            }
            REQUIRE(it_radix == it_radix_end);
        }

        SECTION ("can't insert duplicate values") {
            for (int i = 0; i < size; i++) {
                auto p = radix_set.emplace(data[i]);
                REQUIRE_FALSE(p.second);
            }
        }
    }
}

TEST_CASE("Radix set stress test erase by key", "[radix-set-stress]") {
    art::radix_set<int32_t> radix_set;
    std::set<int32_t> std_set;

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
        radix_set.emplace(data[i]);
        std_set.emplace(data[i]);
    }

    std::random_shuffle(data.begin(), data.end());

    auto block_size = 50;
    auto rounds = number_of_elements / block_size;
    for (int i = 0; i < rounds; i++) {
        for (int j = 0; j < block_size; j++) {
            radix_set.erase(data[i * block_size + j]);
            std_set.erase(data[i * block_size + j]);
        }

        REQUIRE(radix_set.size() == std_set.size());
        for (int k = (i+1) * block_size; k < data.size(); k++) {
            REQUIRE(*radix_set.find(data[k]) == *std_set.find(data[k]));
        }

        auto it_radix = radix_set.begin(), it_radix_end = radix_set.end();
        auto it_std = std_set.begin(), it_std_end = std_set.end();
        for (; it_std != it_std_end; ++it_radix, ++it_std) {
            REQUIRE(*it_radix == *it_std);
        }
        REQUIRE(it_radix == it_radix_end);
    }
}

TEST_CASE("Radix set stress test erase by iterator", "[radix-set-stress]") {
    art::radix_set<int32_t> radix_set;
    std::set<int32_t> std_set;

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
        radix_set.emplace(data[i]);
        std_set.emplace(data[i]);
    }

    std::random_shuffle(data.begin(), data.end());

    auto block_size = 50;
    auto rounds = number_of_elements / block_size;
    for (int i = 0; i < rounds; i++) {
        for (int j = 0; j < block_size; j++) {
            radix_set.erase(radix_set.find(data[i * block_size + j]));
            std_set.erase(data[i * block_size + j]);
        }

        REQUIRE(radix_set.size() == std_set.size());
        for (int k = (i+1) * block_size; k < data.size(); k++) {
            REQUIRE(*radix_set.find(data[k]) == *std_set.find(data[k]));
        }

        auto it_radix = radix_set.begin(), it_radix_end = radix_set.end();
        auto it_std = std_set.begin(), it_std_end = std_set.end();
        for (; it_std != it_std_end; ++it_radix, ++it_std) {
            REQUIRE(*it_radix == *it_std);
        }
        REQUIRE(it_radix == it_radix_end);
    }
}

TEST_CASE("Radix set stress test lower & upper bound", "[radix-set-stress]") {
    std::set<int> set;
    art::radix_set<int> art;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-100000, 100000);

    for (int i = 0; i < 10000; i++) {
        auto k = dis(gen);
        set.insert(k);
        art.insert(k);
    }

    SECTION ("lower bound") {
        for (int i = -100000; i < 100000; i++) {
            auto it_std = set.lower_bound(i);
            auto it_art = art.lower_bound(i);
            if (it_std != set.end()) {
                REQUIRE(*it_art == *it_std);
            } else {
                REQUIRE(it_art == art.end());
            }

            it_std = set.upper_bound(i);
            it_art = art.upper_bound(i);
            if (it_std != set.end()) {
                REQUIRE(*it_art == *it_std);
            } else {
                REQUIRE(it_art == art.end());
            }
        }
    }
}

TEST_CASE("Radix set stress test 16 byte pair", "[radix-set-stress]") {
    std::set<std::pair<uint64_t, uint64_t> > set;
    art::radix_set<std::pair<uint64_t, uint64_t> > radix_set;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> dis(0, std::numeric_limits<uint64_t>::max());

    for (int i = 0; i < 10000; i++) {
        auto first = dis(gen);
        auto second = dis(gen);
        auto std_res = set.insert(std::make_pair(first, second));
        auto radix_res = radix_set.insert(std::make_pair(first, second));
        REQUIRE(radix_res.second == std_res.second);
    }

    SECTION ("maps are equal") {
        REQUIRE(radix_set.size() == set.size());
        REQUIRE(std::equal(set.begin(), set.end(), radix_set.begin()));
    }
}