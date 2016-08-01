# cpp-art [![Build Status](https://travis-ci.com/philipbecker/cpp-art.svg?token=hxeqbVxYkPnTffugJJXK&branch=master)](https://travis-ci.com/philipbecker/cpp-art) [![license](https://img.shields.io/github/license/mashape/apistatus.svg?maxAge=2592000)](https://github.com/philipbecker/cpp-art/blob/master/LICENSE)
cpp-art is a C++11 STL-commpliant map and set container based on an [Adaptive Radix Tree](http://db.in.tum.de/~leis/papers/ART.pdf) (ART) implementation. ART supports O(k) operations (where k is the size of the key in bytes) while limiting the characteristic space overhead of radix trees by changing node sizes. (See the [wiki](https://github.com/philipbecker/cpp-art/wiki) for more information.) The containers provide virtually the same interface as their stl counterparts (see the wiki for the interface documentation: [radix_map](https://github.com/philipbecker/adaptive_radix_tree/wiki/art::radix_map), [radix_set](https://github.com/philipbecker/adaptive_radix_tree/wiki/art::radix_set)) and they meet (mostly/completely?) the requirements of [Container](http://en.cppreference.com/w/cpp/concept/Container), [AssociativeContainer](http://en.cppreference.com/w/cpp/concept/AssociativeContainer) (there is a [key_transform](https://github.com/philipbecker/cpp-art/wiki/art::key_transform) instead of key_comp) and [ReversibleContainer](http://en.cppreference.com/w/cpp/concept/ReversibleContainer). (Please report other deviations as issues.)


# Some (Temporary) Differences & Limitations
* **Only integers are supported as keys** out of the box at the moment. Support for string/char is on the roadmap.
* There is a **`key_transform`** function instead of `key_comp`/`hash` that you can implement for **custom types**. The return type of  `key_transform` **must be an integer type** for now.
* Container is **not thread-safe** (yet)!!!
* [AllocatorAwareContainer](http://en.cppreference.com/w/cpp/concept/AllocatorAwareContainer) is on the roadmap.
* Mapped type needs to be default constructible.
* Erase by iterator only works with non-const iterator.

# Getting Started
cpp-art is a header-only library and requires C++11 features. Simply add `<path-of-your-choice>/cpp-art/include` to your include paths and then the relevant headers can then be included with `#include <art/radix_set.h>` and `#include <art/radix_map.h>`. For further information, please take a look at the [wiki](https://github.com/philipbecker/cpp-art/wiki).

# (Preliminary) Benchmarks
 * **Sparse**: Sampled uniform at random of the entire key type's domain.
 * **Dense**: Sequential keys 0..16M.
 * Comparison of std::map, std::unordered_map, google's cpp-btree and cpp-art.
 * Preliminary because of lacking due diligence when measuring and, seeing as some main features are still missing in cpp-art, there also hasn't been performance optimization. Also, variance is very high, in particular for the radix tree.
 
## Insert 1.6M Elements into a Container with 16M Elements (64-Bit Keys)
![](benchmarks/charts/insert-10.png?raw=true)
## Random Lookup of Valid 64-Bit (left) and 32-Bit (right) Keys in a Container with 16M Elements
Looking up non-existing keys is up to 3-5 times faster in a radix tree, other containers are unaffected or gain much less.
![](benchmarks/charts/lookup-64.png?raw=true) ![](benchmarks/charts/lookup-32.png?raw=true)
## Space Overhead of a Container 16M Elements
![](benchmarks/charts/space-overhead.png?raw=true)
