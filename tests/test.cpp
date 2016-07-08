#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "../libs/catch.h"
#include "../adapt_radix_tree.h"

TEST_CASE( "Can Insert", "[art]" ) {
    adapt_radix_tree art;
    art.insert(5);
    REQUIRE( art.find(5) );
    REQUIRE_FALSE( art.find(6) );
}

TEST_CASE( "Can't insert duplicate values", "[art]" ) {
    adapt_radix_tree art;

    for (int i = 0; i < 20000; i++) {
        auto p = art.insert(i);
        REQUIRE( p.second );
        REQUIRE( art.find(i) );
    }

    for (int i = 0; i < 20000; i++) {
        auto p = art.insert(i);
        REQUIRE_FALSE( p.second );
        REQUIRE( art.find(i) );
    }
}


TEST_CASE( "Can tiebreak at level 1", "[art]" ) {
    adapt_radix_tree art;
    art.insert(5);
    art.insert(6);
    art.insert(261);
    REQUIRE( art.find(5)  );
    REQUIRE( art.find(261) );
}

TEST_CASE( "Can tiebreak at root level", "[art]" ) {
    adapt_radix_tree art;
    art.insert(5);
    art.insert(261);
    REQUIRE( art.find(5)  );
    REQUIRE( art.find(261) );
}

TEST_CASE( "Can tiebreak at root for 2 levels", "[art]" ) {
    adapt_radix_tree art;
    art.insert(5);
    art.insert(65541);
    REQUIRE( art.find(5)  );
    REQUIRE( art.find(65541) );
}

TEST_CASE( "Can tiebreak at root for 3 levels", "[art]" ) {
    adapt_radix_tree art;
    art.insert(5);
    art.insert(16777221);
    REQUIRE( art.find(5) );
    REQUIRE( art.find(16777221) );
}

TEST_CASE( "Insert 1..100k % 3", "[art]" ) {
    adapt_radix_tree art;
    for (int i = 0; i < 1000000; i++)
        if (i % 3 == 0)
            art.insert(i);

    for (int i = 0; i < 1000000; i++) {
        if (i % 3 == 0)
            REQUIRE( art.find(i) );
        else
            REQUIRE_FALSE( art.find(i) );
    }
}
