#include <map>
#include "../libs/catch.h"
#include "../Adaptive_radix_tree.h"


SCENARIO("basic iteration", "[iterator]") {
    art::Adaptive_radix_tree<unsigned, unsigned> art;
    std::map<unsigned, unsigned> std_map;

    for (unsigned i = 1; i < 10; i++) {
        art.insert(std::make_pair(i, i));
        std_map.emplace(i, i);
    }

    CHECK(*(art.begin()) == std_map.begin()->first);
    CHECK(*(art.rbegin()) == std_map.rbegin()->first);

}

SCENARIO("given an art with signed integer key", "[iterator]") {
    art::Adaptive_radix_tree<int, int> art;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-1000000, 1000000);

    std::vector<int> data;
    for (int i = 0; i < 10000; i++) {
        auto candidate = dis(gen);
        data.push_back(candidate);
    }

    for (auto &d : data)
        art.insert(std::make_pair(d, d));

    THEN("forward iteration is sorted correctly") {
        REQUIRE(std::is_sorted(art.begin(), art.end()));
    }

    THEN("reverse iteration is sorted correctly") {
        REQUIRE(std::is_sorted(art.rbegin(), art.rend(), [](int a, int b) {
            return a > b;
        }));
    }

}

SCENARIO("given an art with unsigned integer key", "[iterator]") {
    art::Adaptive_radix_tree<unsigned, unsigned> art;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1000000);

    std::vector<unsigned> data;
    for (int i = 0; i < 10000; i++) {
        auto candidate = (unsigned) dis(gen);
        data.push_back(candidate);
    }

    for (auto &d : data)
        art.insert(std::make_pair(d, d));

    THEN("forward iteration is sorted correctly") {
        REQUIRE(std::is_sorted(art.begin(), art.end()));
    }
    THEN("reverse iteration is sorted correctly") {
        REQUIRE(std::is_sorted(art.rbegin(), art.rend(), [](unsigned a, unsigned b) {
            return a > b;
        }));
    }
}