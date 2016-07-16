# Adaptive Radix Tree (ART)
STL-like map, multimap and set container based on an Adaptive Radix Tree (ART) implementation.

## TODO
### art
* functionality:
    * emplace
    * erase
    * clear
    * upper_bound
    * swap
    * operator=
    * destructor
    * Allocator: allocation, deallocation, memory leaks...
* performance improvements:
    * implement prefix compression
    * consider different leaf implementations
    * consider different iterator models
#### keys
* variable key length support
* binary comparable key transform
    * char*
    * std::string
    * float/double

### interfaces
* for now: map
* for later: set & multimap

### tests
* add lots of stress tests to ensure equality of behaviour with std::map

### benchmarks
* for all key types, compare with std rb-tree & hashmap
    * insertion
    * lookup
    * deletion
    * iteration
    * (assignemnt, copy)
* compare fat iterators, lightweight iterators & leaf-to-leaf pointers

### memory profil
* find tool for memory profiling
* compare with std rb-tree & hashmap
