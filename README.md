# Adaptive Radix Tree (ART)
STL-like map, multimap and set container based on an Adaptive Radix Tree (ART) implementation.

## TODO
### art
* Allocator: allocation, deallocation, memory leaks...
* performance improvements:
    * implement path compression
    * consider different leaf implementations

### keys
* variable key length support
* binary comparable key transform for: char*, std::string

### interfaces
- [ ] map
- [ ] set

### Open Issues
* Mapped type needs to be default constructible due to stupid dummy node implementation

### benchmarks & memory profiling
* compare with std::map, std::unordered_map and google cpp-btree

## STL-Compliance
Container is not **thread safe** (yet)!!!

art::radix_map meets the requirements of [Container](http://en.cppreference.com/w/cpp/concept/Container), [AssociativeContainer](http://en.cppreference.com/w/cpp/concept/AssociativeContainer) (for the most part, there is a key_transform instead of key_comp) and [ReversibleContainer](http://en.cppreference.com/w/cpp/concept/ReversibleContainer). 

[AllocatorAwareContainer](http://en.cppreference.com/w/cpp/concept/AllocatorAwareContainer) is on the roadmap.