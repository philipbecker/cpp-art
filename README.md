# cpp-art [![Build Status](https://travis-ci.com/philipbecker/cpp-art.svg?token=hxeqbVxYkPnTffugJJXK&branch=master)](https://travis-ci.com/philipbecker/cpp-art)
cpp-art is a C++11 STL-like map and set container based on an [Adaptive Radix Tree](http://db.in.tum.de/~leis/papers/ART.pdf) (ART) implementation. ART supports O(k) operations (where k is the size of the key in bytes) while limiting the characteristic space overhead of radix trees by changing node sizes. (See the [wiki](https://github.com/philipbecker/cpp-art/wiki) for more information). The containers can almsot seamlessly replace their STL counterparts, their interface is almost identical. (Full interface documentation: [radix_map](https://github.com/philipbecker/adaptive_radix_tree/wiki/art::radix_map), [radix_set](https://github.com/philipbecker/adaptive_radix_tree/wiki/art::radix_set)). They meet the requirements of [Container](http://en.cppreference.com/w/cpp/concept/Container), [AssociativeContainer](http://en.cppreference.com/w/cpp/concept/AssociativeContainer) (for the most part, there is a key_transform instead of key_comp) and [ReversibleContainer](http://en.cppreference.com/w/cpp/concept/ReversibleContainer). (Please report other deviations as issues.)

# (Preliminary) Benchmarks

# Installation
Requires C++11 features.
Add `cpp-art/include` to your inclusions.

# Some Differences
* Container is **not** thread-safe (yet)!!!
* There is a `key_transform` function instead of `key_comp`/`hash`.
* Return type `key_transform` **must be an integer type** for now. Support for string/char is on the roadmap.
* [AllocatorAwareContainer](http://en.cppreference.com/w/cpp/concept/AllocatorAwareContainer) is on the roadmap.
* Mapped type needs to be default constructible.
* Erase by iterator only works with non-const iterator.
