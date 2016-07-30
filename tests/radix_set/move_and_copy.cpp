#include <map>
#include "catch.hpp"
#include "art/radix_map.h"

TEST_CASE("Radix set move Constructor", "[radix-set]") {
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

TEST_CASE("Radix set move Assignment", "[radix-set]") {
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

TEST_CASE("Radix set move container assignment is valid after source has been destroyed", "[radix-set]") {
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

TEST_CASE("Radix set swap", "[radix-set]") {
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

TEST_CASE("Radix set copy constructor", "[radix-set]") {
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

TEST_CASE("Radix set copy assignment", "[radix-set]") {
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

TEST_CASE("Radix set copy container assignment is valid after source has been destroyed", "[radix-set]") {
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

TEST_CASE("Radix set copy construct with empty source", "[radix-set]") {
    art::radix_map<int, int> reference;
    art::radix_map<int, int> source;
    art::radix_map<int, int> target(source);

    REQUIRE(target.size() == reference.size());
    source.emplace(1, 2);
    REQUIRE(target.size() == reference.size());
    target.emplace(4, 2);
    source.clear();
    REQUIRE(target.size() == 1);
    REQUIRE(source.size() == 0);
}

TEST_CASE("Radix set copy assign with empty source", "[radix-set]") {
    art::radix_map<int, int> reference;
    art::radix_map<int, int> source;
    art::radix_map<int, int> target;
    target = source;

    REQUIRE(target.size() == reference.size());
    source.emplace(1, 2);
    REQUIRE(target.size() == reference.size());
    target.emplace(4, 2);
    source.clear();
    REQUIRE(target.size() == 1);
    REQUIRE(source.size() == 0);
}

TEST_CASE("Radix set move construct with empty source", "[radix-set]") {
    art::radix_map<int, int> reference;
    art::radix_map<int, int> source;
    art::radix_map<int, int> target(std::move(source));


    REQUIRE(target.size() == reference.size());
    source.emplace(1, 2);
    REQUIRE(target.size() == reference.size());
    target.emplace(4, 2);
    source.clear();
    REQUIRE(target.size() == 1);
    REQUIRE(source.size() == 0);
}

TEST_CASE("Radix set move assign with empty source", "[radix-set]") {
    art::radix_map<int, int> reference;
    art::radix_map<int, int> source;
    art::radix_map<int, int> target;
    target = std::move(source);

    REQUIRE(target.size() == reference.size());
    source.emplace(1, 2);
    REQUIRE(target.size() == reference.size());
    target.emplace(4, 2);
    source.clear();
    REQUIRE(target.size() == 1);
    REQUIRE(source.size() == 0);
}

