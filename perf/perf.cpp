#include <celero/Celero.h>
#include "../src/Adaptive_radix_tree.h"
#include <map>

std::map<int, int> map;

BENCHMARK(Map, insert, 10, 2000) {
    map.insert(std::make_pair(1, 2));
}