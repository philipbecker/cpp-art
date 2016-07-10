#include "../libs/catch.h"
#include "../adapt_radix_tree.h"

/**
TEST_CASE("Can't insert duplicate values", "[art]") {
    adapt_radix_tree art;

    for (int i = 0; i < 20000; i++) {
        auto p = art.insert(i);
        REQUIRE(p.second);
        REQUIRE(art.find(i));
    }

    for (int i = 0; i < 20000; i++) {
        auto p = art.insert(i);
        REQUIRE_FALSE(p.second);
        REQUIRE(art.find(i));
    }
}

TEST_CASE("Insert 1..100k % 3", "[art]") {
    adapt_radix_tree art;
    for (int i = 0; i < 1000000; i++)
        if (i % 3 == 0)
            art.insert(i);

    for (int i = 0; i < 1000000; i++) {
        if (i % 3 == 0)
            REQUIRE(art.find(i));
        else
            REQUIRE_FALSE(art.find(i));
    }
}
 */