#include "catch.hpp"
#include "../src/Adaptive_radix_tree.h"
#include "../src/radix_map.h"


TEST_CASE("Stress tests", "[stress]") {
    art::radix_map<int, int> art;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 250000);

    std::vector<int> data;
    for (int i = 0; i < 10000; i++) {
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

        SECTION ("exactly the inserted values were inserted") {
            for (int i = 0; i < 250000; i++) {
                auto it = std::find(data.begin(), data.end(), i);
                if (it != data.end())
                    REQUIRE(art.find(i) != art.end());
                else
                    REQUIRE(art.find(i) == art.end());
            }
        }

        SECTION ("can't insert duplicate values") {
            for (int i = 0; i < size; i++) {
                auto p = art.insert(std::make_pair(data[i], data[i]));
                REQUIRE_FALSE(p.second);
            }
        }
    }
}
