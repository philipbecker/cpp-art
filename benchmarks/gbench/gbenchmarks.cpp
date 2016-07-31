#include <benchmark/benchmark.h>

#include <map>
#include <unordered_map>
#include <btree_map.h>
#include <art/radix_map.h>

const int START = 24;
const int END = 24;

template<typename T>
struct Generator {
    std::mt19937 gen;
    std::uniform_int_distribution<T> dis;

    Generator() {
        std::random_device rd;
        gen = std::mt19937(rd());
        dis = std::uniform_int_distribution<T>(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
    }

    T next() {
        return dis(gen);
    }
};

namespace
{
    template<typename Container>
    Container ConstructRandomMap(int size) {
        typedef typename std::remove_const<typename Container::key_type>::type K;

        Container m;
        Generator<K> key_generator;
        for (int i = 0; i < size; ++i) {
            m.insert(std::make_pair(key_generator.next(), key_generator.next()));
        }
        return m;
    }

    template<typename Container>
    Container ConstructDenseMap(int size) {
        typedef typename std::remove_const<typename Container::key_type>::type K;

        Container m;
        Generator<K> key_generator;
        for (int i = 0; i < size; ++i) {
            m.insert(std::make_pair(i, key_generator.next()));
        }
        return m;
    }

}  // namespace

template<typename Container>
static void BM_Lookup_Sparse(benchmark::State &state) {
    typedef typename std::remove_const<typename Container::key_type>::type K;
    typedef typename std::remove_const<typename Container::value_type>::type V;

    const int size = state.range_x();
    Generator<K> key_generator;
    while (state.KeepRunning()) {
        state.PauseTiming();
        Container m = ConstructRandomMap<Container>(size);
        state.ResumeTiming();
        for (int i = 0; i < size; ++i) {
            benchmark::DoNotOptimize(m.find(key_generator.next()));
        }
    }
    const size_t items_processed = state.iterations() * state.range_x();
    state.SetItemsProcessed(items_processed);
    //state.SetBytesProcessed(items_processed * sizeof(V));
}

template<typename Container>
static void BM_Lookup_Sparse_Valid(benchmark::State &state) {
    typedef typename std::remove_const<typename Container::key_type>::type K;
    typedef typename std::remove_const<typename Container::value_type>::type V;

    const int size = state.range_x();
    while (state.KeepRunning()) {
        state.PauseTiming();
        Container m = ConstructRandomMap<Container>(size);
        std::vector<K> keys;
        for (auto &e : m)
            keys.push_back(e.first);
        std::random_shuffle(keys.begin(), keys.end());
        const size_t key_size = keys.size();
        state.ResumeTiming();
        for (int i = 0; i < key_size; ++i) {
            benchmark::DoNotOptimize(m.find(keys[i]));
        }
    }
    const size_t items_processed = state.iterations() * state.range_x();
    state.SetItemsProcessed(items_processed);
    //state.SetBytesProcessed(items_processed * sizeof(V));
}

template<typename Container>
static void BM_Lookup_Dense(benchmark::State &state) {
    typedef typename std::remove_const<typename Container::key_type>::type K;
    typedef typename std::remove_const<typename Container::value_type>::type V;

    const int size = state.range_x();
    while (state.KeepRunning()) {
        state.PauseTiming();
        Container m = ConstructDenseMap<Container>(size);
        Generator<K> key_generator;
        state.ResumeTiming();
        for (int i = 0; i < size; ++i) {
            benchmark::DoNotOptimize(m.find(key_generator.next()));
        }
    }
    const size_t items_processed = state.iterations() * state.range_x();
    state.SetItemsProcessed(items_processed);
    //state.SetBytesProcessed(items_processed * sizeof(V));
}

template<typename Container>
static void BM_Lookup_Dense_Valid(benchmark::State &state) {
    typedef typename std::remove_const<typename Container::key_type>::type K;
    typedef typename std::remove_const<typename Container::value_type>::type V;

    const int size = state.range_x();
    while (state.KeepRunning()) {
        state.PauseTiming();
        Container m = ConstructDenseMap<Container>(size);
        std::vector<K> keys;
        for (auto &e : m)
            keys.push_back(e.first);
        std::random_shuffle(keys.begin(), keys.end());
        const size_t key_size = keys.size();
        state.ResumeTiming();
        for (int i = 0; i < key_size; ++i) {
            benchmark::DoNotOptimize(m.find(keys[i]));
        }
    }
    const size_t items_processed = state.iterations() * state.range_x();
    state.SetItemsProcessed(items_processed);
    //state.SetBytesProcessed(items_processed * sizeof(V));
}

template<typename Container>
static void BM_Insert_Sparse(benchmark::State &state) {
    typedef typename std::remove_const<typename Container::key_type>::type K;
    typedef typename std::remove_const<typename Container::value_type>::type V;
    const int size = state.range_x();

    while (state.KeepRunning()) {
        state.PauseTiming();
        Container m = ConstructRandomMap<Container>(size);

        std::vector<K> values;
        for (auto &e : m)
            values.push_back(e.first);

        // Remove & re-insert a different 10% from the same map for 10 times
        for (int i = 0; i < 10; i++) {
            std::random_shuffle(values.begin(), values.end());
            const size_t ten_percent = values.size() / 10;

            for (int j = 0; j < ten_percent; j++)
                m.erase(values[j]);

            state.ResumeTiming();
            for (int j = 0; j < ten_percent; j++)
                m.insert(std::make_pair(values[j], 1));
            state.PauseTiming();
        }
    }
    const size_t items_processed = state.iterations() * state.range_x() / 10;
    state.SetItemsProcessed(items_processed);
}

template<typename Container>
static void BM_Insert_Dense(benchmark::State &state) {
    typedef typename std::remove_const<typename Container::key_type>::type K;
    typedef typename std::remove_const<typename Container::value_type>::type V;
    const int size = state.range_x();

    while (state.KeepRunning()) {
        state.PauseTiming();
        Container m = ConstructDenseMap<Container>(size);

        std::vector<K> values;
        for (auto &e : m)
            values.push_back(e.first);

        // Remove & re-insert a different 10% from the same map for 10 times
        for (int i = 0; i < 10; i++) {
            std::random_shuffle(values.begin(), values.end());
            const size_t ten_percent = values.size() / 10;

            for (int j = 0; j < ten_percent; j++)
                m.erase(values[j]);

            state.ResumeTiming();
            for (int j = 0; j < ten_percent; j++)
                m.insert(std::make_pair(values[j], 1));
            state.PauseTiming();
        }
    }
    const size_t items_processed = state.iterations() * state.range_x() / 10;
    state.SetItemsProcessed(items_processed);
}

template<typename Container>
static void BM_Insert_Sequential(benchmark::State &state) {
    typedef typename std::remove_const<typename Container::key_type>::type K;
    typedef typename std::remove_const<typename Container::value_type>::type V;
    const int size = state.range_x();

    Container m;
    Generator<K> key_generator;

    while (state.KeepRunning()) {
        state.PauseTiming();
        for (int i = 0; i < size; i++) {
            m.insert(std::make_pair(i, key_generator.next()));
        }

        std::vector<K> values;
        for (auto &e : m)
            values.push_back(e.first);

        // Remove & re-insert the first 10% of the map
        for (int i = 0; i < 10; i++) {
            const size_t ten_percent = values.size() / 10;

            for (int j = 0; j < ten_percent; j++)
                m.erase(values[j]);

            state.ResumeTiming();
            for (int j = 0; j < ten_percent; j++)
                m.insert(std::make_pair(values[j], 1));
            state.PauseTiming();
        }
    }
    const size_t items_processed = state.iterations() * state.range_x() / 10;
    state.SetItemsProcessed(items_processed);
}

template<typename Container>
static void BM_Erase_Sparse(benchmark::State &state) {
    typedef typename std::remove_const<typename Container::key_type>::type K;
    typedef typename std::remove_const<typename Container::value_type>::type V;
    const int size = state.range_x();

    while (state.KeepRunning()) {
        state.PauseTiming();
        Container m = ConstructRandomMap<Container>(size);

        std::vector<K> values;
        for (auto &e : m)
            values.push_back(e.first);

        // Remove & re-insert a different 10% from the same map for 10 times
        for (int i = 0; i < 10; i++) {
            std::random_shuffle(values.begin(), values.end());
            const size_t ten_percent = values.size() / 10;

            state.ResumeTiming();
            for (int j = 0; j < ten_percent; j++)
                m.erase(values[j]);
            state.PauseTiming();

            for (int j = 0; j < ten_percent; j++)
                m.insert(std::make_pair(values[j], 1));
        }
    }
    const size_t items_processed = state.iterations() * state.range_x() / 10;
    state.SetItemsProcessed(items_processed);
}

template<typename Container>
static void BM_Erase_Dense(benchmark::State &state) {
    typedef typename std::remove_const<typename Container::key_type>::type K;
    typedef typename std::remove_const<typename Container::value_type>::type V;
    const int size = state.range_x();

    while (state.KeepRunning()) {
        state.PauseTiming();
        Container m = ConstructDenseMap<Container>(size);

        std::vector<K> values;
        for (auto &e : m)
            values.push_back(e.first);

        // Remove & re-insert a different 10% from the same map for 10 times
        for (int i = 0; i < 10; i++) {
            std::random_shuffle(values.begin(), values.end());
            const size_t ten_percent = values.size() / 10;

            state.ResumeTiming();
            for (int j = 0; j < ten_percent; j++)
                m.erase(values[j]);
            state.PauseTiming();

            for (int j = 0; j < ten_percent; j++)
                m.insert(std::make_pair(values[j], 1));
        }
    }
    const size_t items_processed = state.iterations() * state.range_x() / 10;
    state.SetItemsProcessed(items_processed);
}

template<typename Container>
static void BM_Erase_Sequential(benchmark::State &state) {
    typedef typename std::remove_const<typename Container::key_type>::type K;
    typedef typename std::remove_const<typename Container::value_type>::type V;
    const int size = state.range_x();

    Container m;
    Generator<K> key_generator;

    while (state.KeepRunning()) {
        state.PauseTiming();
        for (int i = 0; i < size; i++) {
            m.insert(std::make_pair(i, key_generator.next()));
        }

        std::vector<K> values;
        for (auto &e : m)
            values.push_back(e.first);

        // Remove & re-insert the first 10% of the map
        for (int i = 0; i < 10; i++) {
            const size_t ten_percent = values.size() / 10;

            state.ResumeTiming();
            for (int j = 0; j < ten_percent; j++)
                m.erase(values[j]);
            state.PauseTiming();

            for (int j = 0; j < ten_percent; j++)
                m.insert(std::make_pair(values[j], 1));
        }
    }
    const size_t items_processed = state.iterations() * state.range_x();
    state.SetItemsProcessed(items_processed);
}

template<typename Container>
static void BM_Iteration_Sparse(benchmark::State &state) {
    typedef typename std::remove_const<typename Container::value_type>::type V;

    const int size = state.range_x();
    while (state.KeepRunning()) {
        state.PauseTiming();
        Container m = ConstructRandomMap<Container>(size);
        state.ResumeTiming();
        long sum = 0;
        for (const auto &e : m) {
            benchmark::DoNotOptimize(sum += e.second);
        }
    }
    const size_t items_processed = state.iterations() * state.range_x();
    state.SetItemsProcessed(items_processed);
    state.SetBytesProcessed(items_processed * sizeof(V));
}

template<typename Container>
static void BM_Iteration_Dense(benchmark::State &state) {
    typedef typename std::remove_const<typename Container::value_type>::type V;

    const int size = state.range_x();
    while (state.KeepRunning()) {
        state.PauseTiming();
        Container m = ConstructDenseMap<Container>(size);
        state.ResumeTiming();
        long sum = 0;
        for (const auto &e : m) {
            benchmark::DoNotOptimize(sum += e.second);
        }
    }
    const size_t items_processed = state.iterations() * state.range_x();
    state.SetItemsProcessed(items_processed);
    state.SetBytesProcessed(items_processed * sizeof(V));
}
BENCHMARK_TEMPLATE(BM_Lookup_Dense_Valid, btree::btree_map<int64_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(5.0);

BENCHMARK_TEMPLATE(BM_Lookup_Dense_Valid, art::radix_map<int32_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(5.0);

BENCHMARK_TEMPLATE(BM_Lookup_Dense_Valid, btree::btree_map<int32_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(5.0);

BENCHMARK_TEMPLATE(BM_Lookup_Dense_Valid, art::radix_map<int64_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(5.0);

////////////
// INSERT //
////////////
const double MIN_INSERT_TIME = 2.0;

BENCHMARK_TEMPLATE(BM_Insert_Sparse, std::map<int64_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(MIN_INSERT_TIME);
BENCHMARK_TEMPLATE(BM_Insert_Sparse, std::unordered_map<int64_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(MIN_INSERT_TIME);
BENCHMARK_TEMPLATE(BM_Insert_Sparse, btree::btree_map<int64_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(MIN_INSERT_TIME);
BENCHMARK_TEMPLATE(BM_Insert_Sparse, art::radix_map<int64_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(MIN_INSERT_TIME);

BENCHMARK_TEMPLATE(BM_Insert_Sequential, std::map<int64_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(MIN_INSERT_TIME);
BENCHMARK_TEMPLATE(BM_Insert_Sequential, std::unordered_map<int64_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(MIN_INSERT_TIME);
BENCHMARK_TEMPLATE(BM_Insert_Sequential, btree::btree_map<int64_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(MIN_INSERT_TIME);
BENCHMARK_TEMPLATE(BM_Insert_Sequential, art::radix_map<int64_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(MIN_INSERT_TIME);


////////////
// LOOKUP //
////////////

const double MIN_LOOKUP_TIME = 5.0;
/**
BENCHMARK_TEMPLATE(BM_Lookup_Sparse_Valid, art::radix_map<int64_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(MIN_LOOKUP_TIME);
BENCHMARK_TEMPLATE(BM_Lookup_Dense_Valid, art::radix_map<int64_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(MIN_LOOKUP_TIME);
*/


/**
// SPARSE INT32 KEY
//BENCHMARK_TEMPLATE(BM_Lookup_Sparse, std::map<int32_t, int>)->Range(1 << START, 1 << END);
//BENCHMARK_TEMPLATE(BM_Lookup_Sparse, std::unordered_map<int32_t, int>)->Range(1 << START, 1 << END);
//BENCHMARK_TEMPLATE(BM_Lookup_Sparse, btree::btree_map<int32_t, int>)->Range(1 << START, 1 << END);
//BENCHMARK_TEMPLATE(BM_Lookup_Sparse, art::radix_map<int32_t, int>)->Range(1 << START, 1 << END);
*/
/**
// SPARSE INT64 KEY
//BENCHMARK_TEMPLATE(BM_Lookup_Sparse, std::map<int64_t, int>)->Range(1 << START, 1 << END);
//BENCHMARK_TEMPLATE(BM_Lookup_Sparse, std::unordered_map<int64_t, int>)->Range(1 << START, 1 << END);
//BENCHMARK_TEMPLATE(BM_Lookup_Sparse, btree::btree_map<int64_t, int>)->Range(1 << START, 1 << END);
//BENCHMARK_TEMPLATE(BM_Lookup_Sparse, art::radix_map<int64_t, int>)->Range(1 << START, 1 << END);
*/
// SPARSE INT32 KEY: VALID KEYS ONLY
BENCHMARK_TEMPLATE(BM_Lookup_Sparse_Valid, std::map<int32_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(MIN_LOOKUP_TIME);
BENCHMARK_TEMPLATE(BM_Lookup_Sparse_Valid, std::unordered_map<int32_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(MIN_LOOKUP_TIME);
BENCHMARK_TEMPLATE(BM_Lookup_Sparse_Valid, btree::btree_map<int32_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(MIN_LOOKUP_TIME);
BENCHMARK_TEMPLATE(BM_Lookup_Sparse_Valid, art::radix_map<int32_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(MIN_LOOKUP_TIME);

// SPARSE INT64 KEY: VALID KEYS ONLY
BENCHMARK_TEMPLATE(BM_Lookup_Sparse_Valid, std::map<int64_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(MIN_LOOKUP_TIME);
BENCHMARK_TEMPLATE(BM_Lookup_Sparse_Valid, std::unordered_map<int64_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(MIN_LOOKUP_TIME);
BENCHMARK_TEMPLATE(BM_Lookup_Sparse_Valid, btree::btree_map<int64_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(MIN_LOOKUP_TIME);
BENCHMARK_TEMPLATE(BM_Lookup_Sparse_Valid, art::radix_map<int64_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(MIN_LOOKUP_TIME);

/**
// DENSE INT64 KEY
BENCHMARK_TEMPLATE(BM_Lookup_Dense, std::map<int64_t, int>)->Range(1 << START, 1 << END);
BENCHMARK_TEMPLATE(BM_Lookup_Dense, std::unordered_map<int64_t, int>)->Range(1 << START, 1 << END);
BENCHMARK_TEMPLATE(BM_Lookup_Dense, btree::btree_map<int64_t, int>)->Range(1 << START, 1 << END);
BENCHMARK_TEMPLATE(BM_Lookup_Dense, art::radix_map<int64_t, int>)->Range(1 << START, 1 << END);
*/
// DENSE INT32 KEY: VALID KEYS ONLY
BENCHMARK_TEMPLATE(BM_Lookup_Dense_Valid, std::map<int32_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(MIN_LOOKUP_TIME);

BENCHMARK_TEMPLATE(BM_Lookup_Dense_Valid, std::unordered_map<int32_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(MIN_LOOKUP_TIME);

BENCHMARK_TEMPLATE(BM_Lookup_Dense_Valid, btree::btree_map<int32_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(MIN_LOOKUP_TIME);

BENCHMARK_TEMPLATE(BM_Lookup_Dense_Valid, art::radix_map<int32_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(MIN_LOOKUP_TIME);


// DENSE INT64 KEY: VALID KEYS ONLY
BENCHMARK_TEMPLATE(BM_Lookup_Dense_Valid, std::map<int64_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(MIN_LOOKUP_TIME);

BENCHMARK_TEMPLATE(BM_Lookup_Dense_Valid, std::unordered_map<int64_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(MIN_LOOKUP_TIME);

BENCHMARK_TEMPLATE(BM_Lookup_Dense_Valid, btree::btree_map<int64_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(MIN_LOOKUP_TIME);

BENCHMARK_TEMPLATE(BM_Lookup_Dense_Valid, art::radix_map<int64_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(MIN_LOOKUP_TIME);


///////////
// ERASE //
///////////

const double MIN_ERASE_TIME = 2.0;

/*
BENCHMARK_TEMPLATE(BM_Erase_Sparse, std::map<int64_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(MIN_ERASE_TIME);
BENCHMARK_TEMPLATE(BM_Erase_Sparse, std::unordered_map<int64_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(MIN_ERASE_TIME);
BENCHMARK_TEMPLATE(BM_Erase_Sparse, btree::btree_map<int64_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(MIN_ERASE_TIME);
BENCHMARK_TEMPLATE(BM_Erase_Sparse, art::radix_map<int64_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(MIN_ERASE_TIME);

BENCHMARK_TEMPLATE(BM_Erase_Dense, std::map<int64_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(MIN_ERASE_TIME);
BENCHMARK_TEMPLATE(BM_Erase_Dense, std::unordered_map<int64_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(MIN_ERASE_TIME);
BENCHMARK_TEMPLATE(BM_Erase_Dense, btree::btree_map<int64_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(MIN_ERASE_TIME);
BENCHMARK_TEMPLATE(BM_Erase_Dense, art::radix_map<int64_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(MIN_ERASE_TIME);

BENCHMARK_TEMPLATE(BM_Erase_Sequential, std::map<int64_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(MIN_ERASE_TIME);
BENCHMARK_TEMPLATE(BM_Erase_Sequential, std::unordered_map<int64_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(MIN_ERASE_TIME);
BENCHMARK_TEMPLATE(BM_Erase_Sequential, btree::btree_map<int64_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(MIN_ERASE_TIME);
BENCHMARK_TEMPLATE(BM_Erase_Sequential, art::radix_map<int64_t, int>)
        ->Range(1 << START, 1 << END)
        ->Unit(benchmark::TimeUnit::kMillisecond)
        ->MinTime(MIN_ERASE_TIME);
*/

///////////////
// ITERATION //
///////////////
/**
BENCHMARK_TEMPLATE(BM_Iteration_Sparse, std::map<int, int>)->Range(1 << START, 1 << END);
BENCHMARK_TEMPLATE(BM_Iteration_Sparse, std::unordered_map<int, int>)->Range(1 << START, 1 << END);
BENCHMARK_TEMPLATE(BM_Iteration_Sparse, btree::btree_map<int, int>)->Range(1 << START, 1 << END);
BENCHMARK_TEMPLATE(BM_Iteration_Sparse, art::radix_map<int, int>)->Range(1 << START, 1 << END);

BENCHMARK_TEMPLATE(BM_Iteration_Dense, std::map<int, int>)->Range(1 << START, 1 << END);
BENCHMARK_TEMPLATE(BM_Iteration_Dense, std::unordered_map<int, int>)->Range(1 << START, 1 << END);
BENCHMARK_TEMPLATE(BM_Iteration_Dense, btree::btree_map<int, int>)->Range(1 << START, 1 << END);
BENCHMARK_TEMPLATE(BM_Iteration_Dense, art::radix_map<int, int>)->Range(1 << START, 1 << END);
*/

BENCHMARK_MAIN()