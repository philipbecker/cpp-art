# Adaptive Radix Tree (ART)
STL-like map, multimap and set container based on an Adaptive Radix Tree (ART) implementation.

## TODO
### art
* API:
    * emplace, erase, operator=
* Allocator: allocation, deallocation, memory leaks...
* performance improvements:
    * implement prefix compression
    * consider different leaf implementations

### keys
* variable key length support
* binary comparable key transform for: char*, std::string, * float/double (meh)

### interfaces
* for now: map
* for later: set & multimap

### tests
* add lots of stress tests to ensure equality of behaviour with std::map

### benchmarks
* for all key types, compare with std::map, std::unordered_map (and google btree)
    * insertion, lookup, deletion, iteration, (assignemnt, copy)

### memory profiling
* compare with std::map, std::unordered_map  and (and google btree)

## STL-compliance
Container is not **thread safe** (yet)!!!
### Concept: [Container](http://en.cppreference.com/w/cpp/concept/Container)
- [x] `C()` Constructor
- [ ] `C(a)` Copy constructor
- [ ] `a = b` Move assignment
- [x] `(&a)->~C()` but not really happy with
- [x] `a.begin()`
- [x] `a.end()`
- [x] `a.cbegin()`
- [x] `a.cend()`
- [x] `a == b`
- [x] `a != b`
- [ ] `a.swap(b)` 
- [ ] `swap(a, b)`
- [x] `a.size()`
- [x] ~~`max_size()`~~ (not going to implement, not applicable)
- [x] `a.empty()`

### Concept: [ReversibleContainer](http://en.cppreference.com/w/cpp/concept/ReversibleContainer)
Done!

### Concept: [AllocatorAwareContainer](http://en.cppreference.com/w/cpp/concept/AllocatorAwareContainer)
Not started implementing.

### Concept: [AssociativeContainer](http://en.cppreference.com/w/cpp/concept/AssociativeContainer)
- [x] `X::key_type`
- [x] `X::key_transformer`
- [ ] `X::value_compare`
- [ ] `X(c)`, `X a(x)` Copy Constructor
- [x] `X()`, `X a;`
- [ ] `X(i, j, c)`, `X a(i, j, c);`
- [x] `X(i, j)`, `X a(i, j);` 
- [x] `X(il);`
- [x] `a = il`
- [ ] `a.key_trans()`
- [ ] `a.value_comp()`
- [ ] support for multiple equal keys

## Sample Code
```C++
// Range constructor
std::vector<std::pair<int, int> > v;
art::radix_map<int, int> map_range(v.begin(), v.end());
        
art::radix_map<unsigned, int> map;
auto p = map.insert(std::pair<unsigned, int>(1, -1)); // p = pair<iterator, bool>
map.insert({2, -2});
map.insert({{3, -3}, {4, -4}});
map[2] = 100;
map[10] = 7;
auto it = map.find(2);
std::cout << "(" << (*it).first << ", " << (*it).second << ")" << std::endl; // (2, 100)
std::cout << map.at(3) << std::endl; // -3
std::cout << map.size() << std::endl; // 5
std::cout << map.empty() << std::endl; // 0
```
