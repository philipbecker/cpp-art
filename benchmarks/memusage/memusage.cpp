#include <unistd.h>
#include <ios>
#include <iostream>
#include <fstream>
#include <random>
#include <map>
#include <unordered_map>
#include <art/radix_map.h>
#include "../../libs/cpp-btree/btree_map.h"

// Source: http://stackoverflow.com/a/671389/1311693

//////////////////////////////////////////////////////////////////////////////
//
// process_mem_usage(double &, double &) - takes two doubles by reference,
// attempts to read the system-dependent data for a process' virtual memory
// size and resident set size, and return the results in KB.
//
// On failure, returns 0.0, 0.0

void process_mem_usage(double &vm_usage, double &resident_set) {
    using std::ios_base;
    using std::ifstream;
    using std::string;

    vm_usage = 0.0;
    resident_set = 0.0;

    // 'file' stat seems to give the most reliable results
    //
    ifstream stat_stream("/proc/self/stat", ios_base::in);

    // dummy vars for leading entries in stat that we don't care about
    //
    string pid, comm, state, ppid, pgrp, session, tty_nr;
    string tpgid, flags, minflt, cminflt, majflt, cmajflt;
    string utime, stime, cutime, cstime, priority, nice;
    string O, itrealvalue, starttime;

    // the two fields we want
    //
    unsigned long vsize;
    long rss;

    stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
    >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
    >> utime >> stime >> cutime >> cstime >> priority >> nice
    >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

    stat_stream.close();

    long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
    vm_usage = vsize / 1024.0;
    resident_set = rss * page_size_kb;
}

int main() {
    using std::cout;
    using std::endl;

    double vm, rss;
    process_mem_usage(vm, rss);
    cout << "VM: " << vm << "; RSS: " << rss << endl;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int64_t> dis(std::numeric_limits<int64_t>::min(),
                                               std::numeric_limits<int64_t>::max());

    std::vector<std::pair<int64_t, int64_t> > data;
    const int size = 16000000;
    for (int i = 0; i < size; i++) {
        int64_t candidate = dis(gen);
        data.emplace_back(candidate, i);
    }

    double key_value_size = size * sizeof(std::pair<int64_t, int64_t>);

    double basline_vm, basline_rss;
    process_mem_usage(basline_vm, basline_rss);
    cout << "-- VM: " << basline_vm << "; RSS: " << basline_rss << endl;

    {
        //std::map<int64_t, int64_t> map;
        //std::unordered_map<int64_t, int64_t> map;
        //btree::btree_map<int64_t, int64_t> map;
        art::radix_map<int64_t, int64_t> map;
        for (auto &d : data)
            map.insert(d);

        double vm, rss;
        process_mem_usage(vm, rss);
        double byte_overhead = ((vm - basline_vm) * 1024) / ((double) size);
        cout << "Overhead/Key in Byte: " << byte_overhead << endl;
    }
}