#include "../libs/catch.h"
#include "../adapt_radix_tree.h"


SCENARIO("given an art", "[iterator]") {
    art::adapt_radix_tree<unsigned, unsigned> art;

    art.insert(std::make_pair(5, 5));
    art.insert(std::make_pair(6, 6));

    art::adapt_radix_tree<unsigned, unsigned>::iterator it = art.begin();
    for (; it != art.end(); it++) {
        std::cout << *it << std::endl;
    }

}
