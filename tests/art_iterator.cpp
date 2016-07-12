#include "../libs/catch.h"
#include "../adapt_radix_tree.h"


SCENARIO("given an art with signed integer key", "[iterator]") {
    art::adapt_radix_tree<int, int> art;

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

    REQUIRE(std::is_sorted(art.begin(), art.end()));
}

SCENARIO("given an art with unsigned integer key", "[iterator]") {
    art::adapt_radix_tree<unsigned, unsigned> art;

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

    REQUIRE(std::is_sorted(art.begin(), art.end()));
}