# cpp-art [![Build Status](https://travis-ci.com/philipbecker/cpp-art.svg?token=hxeqbVxYkPnTffugJJXK&branch=master)](https://travis-ci.com/philipbecker/cpp-art) [![license](https://img.shields.io/github/license/mashape/apistatus.svg?maxAge=2592000)](https://github.com/philipbecker/cpp-art/blob/master/LICENSE)
cpp-art is a C++11 STL-like map and set container based on an [Adaptive Radix Tree](http://db.in.tum.de/~leis/papers/ART.pdf) (ART) implementation. ART supports O(k) operations (where k is the size of the key in bytes) while limiting the characteristic space overhead of radix trees by changing node sizes. (See the [wiki](https://github.com/philipbecker/cpp-art/wiki) for more information). The containers can almsot seamlessly replace their STL counterparts, their interface is almost identical. (Full interface documentation: [radix_map](https://github.com/philipbecker/adaptive_radix_tree/wiki/art::radix_map), [radix_set](https://github.com/philipbecker/adaptive_radix_tree/wiki/art::radix_set)). They meet the requirements of [Container](http://en.cppreference.com/w/cpp/concept/Container), [AssociativeContainer](http://en.cppreference.com/w/cpp/concept/AssociativeContainer) (for the most part, there is a key_transform instead of key_comp) and [ReversibleContainer](http://en.cppreference.com/w/cpp/concept/ReversibleContainer). (Please report other deviations as issues.)


# Some Differences & Limitations
* **Only integers are supported as keys** out of the box at the moment. Support for string/char is on the roadmap.
* There is a **`key_transform`** function instead of `key_comp`/`hash` that you can implement for **custom types**. The return type of  `key_transform` **must be an integer type** for now.
* Container is **not thread-safe** (yet)!!!
* [AllocatorAwareContainer](http://en.cppreference.com/w/cpp/concept/AllocatorAwareContainer) is on the roadmap.
* Mapped type needs to be default constructible.
* Erase by iterator only works with non-const iterator.


# (Preliminary) Benchmarks

# Getting Started
Requires C++11 features.
Add `cpp-art/include` to your inclusions.
