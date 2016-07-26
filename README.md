# cpp-art
cpp-art is an STL-like map and set container based on an [Adaptive Radix Tree](https://www.google.de/url?sa=t&rct=j&q=&esrc=s&source=web&cd=1&cad=rja&uact=8&ved=0ahUKEwin79uKhI_OAhUIkRQKHdJYDaQQFggjMAA&url=http%3A%2F%2Fdb.in.tum.de%2F~leis%2Fpapers%2FART.pdf&usg=AFQjCNEd9GThocFaGFJahA13eqNK70ZjQw&sig2=0PZpcDUUWUT1ha3HvCL6LA) (ART) implementation. A [radix tree](https://en.wikipedia.org/wiki/Radix_tree) is a datastructure that uses the binary representation of a key to store and retrieve values. Instead of comparing keys, like in binary search trees, in each node a chunk of the key (e.g. 8 bit/1 byte) is used to index an array (of size 2^8 = 256) to determine the next child. This means that the complexity of operations like insert, lookup and delete are only dependent on the size of the key (in bytes) and not on the number of elements. On the other hand, this comes at the cost of requiring a lot of memory, since even if the array of 256 has only one child, we need to allocate the whole array. An adaptive radix tree attempts to minimize that space overhead by introducing multiple node sizes that grow/shrink on the fly as more/fewer children are present. This implementation uses nodes of size 4, 16, 48 and 256. Node 4 and 16 are comparison based (only comparing one byte of the key), node 48 uses an indirection array and node 256 just works as described above by simply indexing one array. Furthermore, we employ lazy expansion (removing one-way paths to a leafs) and path compression (removing one-way paths anywhere inside the tree).

# (Preliminary) Benchmarks

# Installation
Add `cpp-art/include` to your inclusions.

# Usage
The containers can almsot seamlessly replace the stl-version, the interface is almost identical. (See their respective wiki pages for the full interface documentation [radix_map](https://github.com/philipbecker/adaptive_radix_tree/wiki/art::radix_map), [radix_set](https://github.com/philipbecker/adaptive_radix_tree/wiki/art::radix_set). They meet the requirements of [Container](http://en.cppreference.com/w/cpp/concept/Container), [AssociativeContainer](http://en.cppreference.com/w/cpp/concept/AssociativeContainer) (for the most part, there is a key_transform instead of key_comp) and [ReversibleContainer](http://en.cppreference.com/w/cpp/concept/ReversibleContainer).

## Some Differences
* Container is **not** thread-safe (yet)!!!
* `key_transform` function instead of `key_comp`/`hash`
* [AllocatorAwareContainer](http://en.cppreference.com/w/cpp/concept/AllocatorAwareContainer) is on the roadmap.
* Erase by iterator only works with non-const iterator

## TODO
### art
* Allocator: allocation, deallocation, memory leaks...
* implement path compression
* variable key length support
* binary comparable key transform for: char*, std::string

### Open Issues
* Mapped type needs to be default constructible

### benchmarks & memory profiling
* compare with std::map, std::unordered_map and google cpp-btree


 

