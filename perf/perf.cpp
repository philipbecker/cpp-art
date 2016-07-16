#include <celero/Celero.h>
#include <algorithm>
#include <map>
#include <unordered_map>
#include "../src/radix_map.h"

class InsertFixture : public celero::TestFixture {
public:
    InsertFixture() {
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
    }

    /// Before each iteration. A common utility function to push back random ints to sort.
    void generate_data() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

        for (int i = 0; i < this->arraySize; i++) {
            auto candidate = dis(gen);
            this->data.emplace_back(i, i);
        }
    }

    std::vector<std::pair<int64_t, int64_t>> data;
    int64_t arraySize;
};

CELERO_MAIN


//
// INSERTION
//
BASELINE_F(InsertInt64, Map, InsertFixture, 3, 5) {
    this->generate_data();
    std::map<int64_t, int64_t> map;

    for (auto &d: this->data)
        map.insert(d);
}

BENCHMARK_F(InsertInt64, UnorderedMap, InsertFixture, 3, 5) {
    this->generate_data();
    std::unordered_map<int64_t, int64_t> map;

    for (auto &d: this->data)
        map.insert(d);
}

BENCHMARK_F(InsertInt64, ArtMap, InsertFixture, 3, 5) {
    this->generate_data();
    art::radix_map<int64_t, int64_t> map;

    for (auto &d: this->data)
        map.insert(d);
}