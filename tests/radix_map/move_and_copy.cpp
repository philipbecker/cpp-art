#include <map>
#include "catch.hpp"
#include "art/radix_map.h"

TEST_CASE("Move Constructor", "[map-move]") {
    art::radix_map<int, int> source;
    art::radix_map<int, int> reference;
    for (int i = -1000; i < 1000; i++) {
        source.insert(std::make_pair(i, i));
        reference.insert(std::make_pair(i, i));
    }

    art::radix_map<int, int> target(std::move(source));
    REQUIRE(target.size() == reference.size());
    auto moved_it = target.begin();
    for (auto it = reference.begin(), end = reference.end(); it != end; it++, moved_it++) {
        REQUIRE(moved_it->first == it->first);
    }
}

TEST_CASE("Move Assignment", "[map-move]") {
    art::radix_map<int, int> target;
    art::radix_map<int, int> reference;
    {
        art::radix_map<int, int> source;
        for (int i = -1000; i < 1000; i++) {
            source.insert(std::make_pair(i, 2 * i));
            reference.insert(std::make_pair(i, 2 * i));
        }

        target = std::move(source);
        SECTION ("target of move is now equal to source map") {
            REQUIRE(target.size() == reference.size());
            auto moved_it = target.begin();
            for (auto it = reference.begin(), end = reference.end(); it != end; it++, moved_it++) {
                REQUIRE(moved_it->first == it->first);
            }
        }
    }
}

TEST_CASE("Move container assignment is valid after source has been destroyed", "[map-move]") {
    art::radix_map<int, int> target;
    art::radix_map<int, int> reference;
    {
        art::radix_map<int, int> source;
        for (int i = -1000; i < 1000; i++) {
            source.insert(std::make_pair(i, i));
            reference.insert(std::make_pair(i, i));
        }

        target = std::move(source);
    } // source destruction

    REQUIRE(target.size() == reference.size());
    auto moved_it = target.begin();
    for (auto it = reference.begin(), end = reference.end(); it != end; it++, moved_it++) {
        REQUIRE(moved_it->first == it->first);
    }
}

TEST_CASE("Swap", "[map-move]") {
    art::radix_map<int, int> a;
    std::map<int, int> reference1;
    for (int i = -1000; i < 1000; i++) {
        a.insert(std::make_pair(i, i));
        reference1.insert(std::make_pair(i, i));
    }

    art::radix_map<int, int> b;
    std::map<int, int> reference2;

    for (int i = -5000; i < 5000; i++) {
        b.insert(std::make_pair(i, i));
        reference2.insert(std::make_pair(i, i));
    }

    art::swap(a, b);

    REQUIRE(a.size() == reference2.size());
    REQUIRE(b.size() == reference1.size());
    auto it = a.begin();
    for (auto it_ref = reference2.begin(), end = reference2.end(); it_ref != end; it_ref++, it++) {
        REQUIRE(it->first == it_ref->first);
    }

    it = b.begin();
    for (auto it_ref = reference1.begin(), end = reference1.end(); it_ref != end; it_ref++, it++) {
        REQUIRE(it->first == it_ref->first);
    }
}

TEST_CASE("Copy Constructor", "[map-move]") {
    art::radix_map<int, int> source;
    art::radix_map<int, int> reference;
    for (int i = -1000; i < 1000; i++) {
        source.insert(std::make_pair(i, 2 * i));
        reference.insert(std::make_pair(i, 2 * i));
    }

    art::radix_map<int, int> target(source);
    REQUIRE(target.size() == source.size());
    auto target_it = target.begin();
    for (auto it = reference.begin(), end = reference.end(); it != end; it++, target_it++) {
        REQUIRE(target_it->first == it->first);
    }
    source.clear();

    REQUIRE(target.size() == reference.size());
    target_it = target.begin();
    for (auto it = reference.begin(), end = reference.end(); it != end; it++, target_it++) {
        REQUIRE(target_it->first == it->first);
    }
}

TEST_CASE("Copy Assignment", "[map-move]") {
    art::radix_map<int, int> target;
    art::radix_map<int, int> reference;

    art::radix_map<int, int> source;
    for (int i = -1000; i < 1000; i++) {
        source.insert(std::make_pair(i, i));
        reference.insert(std::make_pair(i, i));
    }

    target = source;
    SECTION ("target of copy is now equal to source map") {
        REQUIRE(target.size() == reference.size());
        auto moved_it = target.begin();
        for (auto it = reference.begin(), end = reference.end(); it != end; it++, moved_it++) {
            REQUIRE(moved_it->first == it->first);
        }
    }

}

TEST_CASE("Copy container assignment is valid after source has been destroyed", "[map-move]") {
    art::radix_map<int, int> target;
    art::radix_map<int, int> reference;
    {
        art::radix_map<int, int> source;
        for (int i = -1000; i < 1000; i++) {
            source.insert(std::make_pair(i, i));
            reference.insert(std::make_pair(i, i));
        }

        target = source;
    } // source destruction

    REQUIRE(target.size() == reference.size());
    auto moved_it = target.begin();
    for (auto it = reference.begin(), end = reference.end(); it != end; it++, moved_it++) {
        REQUIRE(moved_it->first == it->first);
    }
}

/**
TEST_CASE("Assign construct with empty source", "[map-move]") {
    art::radix_map<int, int> reference;
    art::radix_map<int, int> source;
    art::radix_map<int, int> target(source);


    REQUIRE(target.size() == reference.size());
    source.clear();
    REQUIRE(target.size() == reference.size());
    source.insert(std::make_pair(1, 2));
    REQUIRE(target.size() == reference.size());

}
 */
