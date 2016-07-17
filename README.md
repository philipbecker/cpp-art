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
    * leaf-to-leaf pointers (probably not worth it...)

### keys
* variable key length support
* binary comparable key transform for:
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

### memory profil
* find tool for memory profiling
* compare with std rb-tree & hashmap

## STL-compliance
Container is not **thread safe** (yet)!!!
### Concept: [Container](http://en.cppreference.com/w/cpp/concept/Container)
Missing:
* `a.swap(b)` and `swap(a, b)`
* `max_size()` (not going to implement, not applicable)

### Concept: [ReversibleContainer](http://en.cppreference.com/w/cpp/concept/ReversibleContainer)
Done!

### Concept: [AllocatorAwareContainer](http://en.cppreference.com/w/cpp/concept/AllocatorAwareContainer)
Not started implementing.

### Concept: [AssociativeContainer](http://en.cppreference.com/w/cpp/concept/AssociativeContainer)
Missing:
* `X::key_compare` .. ?
* `X::value_compare`
* no support for multiple equal keys
* .... and lots of other things ;)
