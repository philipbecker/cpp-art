#include "catch.hpp"
#include "../src/Adaptive_radix_tree.h"
#include "../src/radix_map.h"


SCENARIO("given an empty art", "[art]") {
    art::Adaptive_radix_tree<int, int> art;

    THEN("its size is 0") {
        REQUIRE(art.size() == 0);
    }
    WHEN("an element is inserted") {
        art._M_insert_unique(std::make_pair(5, 5));
        THEN("its size is 1 and the inserted value is correct") {
            REQUIRE(art.size() == 1);
            REQUIRE(art.find(5) != art.end());
        }

        AND_WHEN("the same element is inserted again") {
            auto p = art._M_insert_unique(std::make_pair(5, 5));
            THEN("the insertion failed and its size is still 1") {
                REQUIRE_FALSE(p.second);
                REQUIRE(art.size() == 1);
                REQUIRE(art.find(5) != art.end());
            }
        }
    }

    WHEN("two elements with an equal first prefix chunk are inserted") {
        auto p1 = art._M_insert_unique(std::make_pair(5, 5));
        auto p2 = art._M_insert_unique(std::make_pair(261, 261));
        THEN("both elements were inserted successfully") {
            REQUIRE(p1.second);
            REQUIRE(p2.second);
            REQUIRE(art.find(5) != art.end());
            REQUIRE(art.find(261) != art.end());
            REQUIRE(art.size() == 2);
        }
    }

    WHEN("two elements with three equal prefix chunks are inserted") {
        auto p1 = art._M_insert_unique(std::make_pair(5, 5));
        auto p2 = art._M_insert_unique(std::make_pair(16777221, 16777221));
        THEN("both elements were inserted successfully") {
            REQUIRE(p1.second);
            REQUIRE(p2.second);
            REQUIRE(art.find(5) != art.end());
            REQUIRE(art.find(16777221) != art.end());
            REQUIRE(art.size() == 2);
        }
    }
}

TEST_CASE("Can tiebreak at level 1", "[art]") {
    art::Adaptive_radix_tree<int, int> art;
    art._M_insert_unique(std::make_pair(5, 5));
    art._M_insert_unique(std::make_pair(6, 6));
    art._M_insert_unique(std::make_pair(261, 261));
    REQUIRE(art.find(5) != art.end());
    REQUIRE(art.find(6) != art.end());
    REQUIRE(art.find(261) != art.end());
}


SCENARIO("growing the root node", "[art]") {
    art::Adaptive_radix_tree<uint64_t, uint64_t> art;

    std::vector<uint64_t> data(256);
    std::iota(data.begin(), data.end(), 0);
    // The first key byte needs to be decisive,
    // otherwise the root won't grow as desired
    for (auto &d : data)
        d <<= 56;

    WHEN("first 5 values are inserted") {
        for (uint64_t i = 0; i < 5; i++) {
            art._M_insert_unique(std::make_pair(data[i], data[i]));
        }

        THEN("root grows to node 16") {
            REQUIRE(art._M_root->size() == 5);
            REQUIRE(art._M_root->get_type() == 2);
        }

        AND_WHEN("12 more values are inserted") {
            for (uint64_t i = 5; i < 17; i++) {
                art._M_insert_unique(std::make_pair(data[i], data[i]));
            }

            THEN ("root has grown to node 48") {
                REQUIRE(art._M_root->size() == 17);
                REQUIRE(art._M_root->get_type() == 3);
            };
            AND_WHEN("32 more values are inserted") {
                for (uint64_t i = 17; i < 49; i++) {
                    art._M_insert_unique(std::make_pair(data[i], data[i]));
                }

                THEN ("root has grown to node 256") {
                    REQUIRE(art._M_root->size() == 49);
                    REQUIRE(art._M_root->get_type() == 4);
                };
            }
        }
    }
}

TEST_CASE("basic operations", "[art]") {
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

    SECTION ("insert with initializer list") {
        art::radix_map<int, int> art;
        art.insert({{1,   3},
                    {2,   12},
                    {-20, 1512}}
        );

        REQUIRE(art.size() == 3);
        REQUIRE((*art.find(1)).second == 3);
        REQUIRE((*art.find(2)).second == 12);
        REQUIRE((*art.find(-20)).second == 1512);
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
                //    REQUIRE(art.count(v[i].first) == 1);
            }
        }
    }
}