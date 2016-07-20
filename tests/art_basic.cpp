#include <map>
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
            art = {{42,  721},
                   {321, 521},
                   {-51, 21},
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

TEST_CASE("Erase1", "[basic]") {
    std::map<unsigned, unsigned> map;
    art::radix_map<unsigned, unsigned> radix_map;

    for (unsigned i = 0; i < 10000; i++) {
        map.insert(std::make_pair(i, 2*i));
        radix_map.insert(std::make_pair(i, 2*i));
    }

    SECTION ("erase equals std::map") {
        for (unsigned i = 0; i < 10000; i++) {
            auto std_res = map.erase(i);
            auto art_res = radix_map.erase(i);
            //std::cout << "deleted " << i << " " << (std_res ? "true" : "false")
            //<< " " << (art_res ? "true" : "false") << std::endl;
            REQUIRE(std_res == art_res);
            REQUIRE(map.size() == radix_map.size());
        }
    }
}

TEST_CASE("Erase complex", "[basic]") {
    std::map<int, int> map;
    art::radix_map<int, int> radix_map;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-100000, 100000);

    for (int i = 0; i < 10000; i++) {
        auto k = dis(gen);
        map.insert(std::make_pair(k, 2*i));
        radix_map.insert(std::make_pair(k, 2*i));
    }

    SECTION ("erase equals std::map") {
        for (int i = -100000; i < 100000; i++) {
            auto std_res = map.erase(i);
            auto art_res = radix_map.erase(i);
            //std::cout << "deleted " << i << " " << (std_res ? "true" : "false")
            //<< " " << (art_res ? "true" : "false") << std::endl;
            REQUIRE(std_res == art_res);
            REQUIRE(map.size() == radix_map.size());
        }
    }
}


TEST_CASE("Erase with iterators", "[basic]") {
    std::map<int, int> map;
    art::radix_map<int, int> radix_map;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-100000, 100000);

    for (int i = 0; i < 10000; i++) {
        map.insert(std::make_pair(i, 2*i));
        radix_map.insert(std::make_pair(i, 2*i));
    }


    SECTION ("erase iterators from map") {
        auto map_next = map.erase(map.find(54));
        auto radix_next = radix_map.erase(radix_map.find(54));
        REQUIRE(radix_next->first == map_next->first);
        REQUIRE(map.size() == radix_map.size());
        REQUIRE(map.find(54) == map.end());
        REQUIRE(radix_map.find(54) == radix_map.end());
        map.erase(map.find(0));
        radix_map.erase(radix_map.find(0));
        REQUIRE(radix_next->first == map_next->first);
        REQUIRE(map.size() == radix_map.size());
        REQUIRE(map.find(0) == map.end());
        REQUIRE(radix_map.find(0) == radix_map.end());
        map.erase(map.find(9999));
        radix_map.erase(radix_map.find(9999));
        REQUIRE(radix_next->first == map_next->first);
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

TEST_CASE("Operator[]", "[basic]") {
    std::map<int, int> map;
    art::radix_map<int, int> art;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-100000, 100000);

    for (int i = 0; i < 20000; i++) {
        auto k = dis(gen);
        map.insert(std::make_pair(k, 2*i));
        art.insert(std::make_pair(k, 2*i));
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