#include <celero/Celero.h>
#include <algorithm>
#include <map>
#include <unordered_map>
#include "../libs/cpp-btree/btree_map.h"
#include "../src/radix_map.h"
#include "parameters.h"

class IterationFixture : public celero::TestFixture {
public:
    IterationFixture() {
    }

    virtual std::vector<std::pair<int64_t, uint64_t>> getExperimentValues() const override {
        std::vector<std::pair<int64_t, uint64_t>> problemSpace;

        // We will run some total number of sets of tests all together.
        // Each one growing by a power of 2.
        const int totalNumberOfTests = 8;

        for (int i = 0; i < totalNumberOfTests; i++) {
            // ExperimentValues is part of the base class and allows us to specify
            // some values to control various test runs to end up building a nice graph.
            problemSpace.push_back(std::make_pair(int64_t(pow(2, i + 15)), uint64_t(0)));
        }

        return problemSpace;
    }

    /// Before each run, build a vector of random integers.
    virtual void setUp(int64_t experimentValue) {
        this->arraySize = experimentValue;
        this->data.reserve(this->arraySize);
        this->generate_data();
        this->build_map();
        this->build_unorderd_map();
        this->build_radix_map();
        this->build_btree_map();
    }

    /// Before each iteration. A common utility function to push back random ints to sort.
    void generate_data() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

        for (int i = 0; i < this->arraySize; i++) {
            auto candidate = dis(gen);
            this->data.emplace_back(candidate, i);
        }
    }

    void build_map() {
        for (auto &d: this->data)
            map.insert(d);
    }

    void build_unorderd_map() {
        for (auto &d: this->data)
            unordered_map.insert(d);
    }

    void build_radix_map() {
        for (auto &d: this->data)
            radix_map.insert(d);
    }

    void build_btree_map() {
        for (auto &d: this->data)
            btree_map.insert(d);
    }

    std::vector<std::pair<int64_t, int64_t>> data;
    std::map<int64_t, int64_t> map;
    std::unordered_map<int64_t, int64_t> unordered_map;
    art::radix_map<int64_t, int64_t> radix_map;
    btree::btree_map<int64_t, int64_t> btree_map;
    int64_t arraySize;
};

BASELINE_F(IterationInt64, Map, IterationFixture, ITERATION_SAMPLES, ITERATION_ITERATIONS) {
    long sum = 0;
    for (auto &d: this->map)
        sum += d.second;
}

BENCHMARK_F(IterationInt64, UnorderedMap, IterationFixture, ITERATION_SAMPLES, ITERATION_ITERATIONS) {
    long sum = 0;
    for (auto &d: this->unordered_map)
        sum += d.second;
}

BENCHMARK_F(IterationInt64, ArtMap, IterationFixture, ITERATION_SAMPLES, ITERATION_ITERATIONS) {
    long sum = 0;
    for (auto &d: this->radix_map)
        sum += d.second;
}

BENCHMARK_F(IterationInt64, BtreeMap, IterationFixture, ITERATION_SAMPLES, ITERATION_ITERATIONS) {
    long sum = 0;
    for (auto &d: this->btree_map)
        sum += d.second;
}