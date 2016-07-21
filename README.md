# Adaptive Radix Tree (ART)
STL-like map, multimap and set container based on an Adaptive Radix Tree (ART) implementation.

## TODO
### art
* API:
    * emplace
* Allocator: allocation, deallocation, memory leaks...
* performance improvements:
    * implement prefix compression
    * consider different leaf implementations

### keys
* variable key length support
* binary comparable key transform for: char*, std::string

### interfaces
- [ ] map
- [ ] set

### benchmarks & memory profiling
* compare with std::map, std::unordered_map and google cpp-btree

## STL-compliance
Container is not **thread safe** (yet)!!!

art::radix_map meets the requirements of [Container](http://en.cppreference.com/w/cpp/concept/Container), [AssociativeContainer](http://en.cppreference.com/w/cpp/concept/AssociativeContainer) (for the most part, there is a key_transform instead of key_comp) and [ReversibleContainer](http://en.cppreference.com/w/cpp/concept/ReversibleContainer). 

[AllocatorAwareContainer](http://en.cppreference.com/w/cpp/concept/AllocatorAwareContainer) is on the roadmap.

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
## Custom key transformation
```C++
// Signed int key transformation without signed bit flip
// Negative values are now larger than positive ones
struct custom_transform {
    int64_t operator()(const int64_t &key) const noexcept {
        return art::is_big_endian() ? key : art::byte_swap(key);
    }
};

int main() {
    art::radix_map<int64_t, int64_t, custom_transform> map;
    map.insert(std::pair<int64_t, int64_t>(-2, 5));
    map.insert(std::pair<int64_t, int64_t>(0, 5));
    map.insert(std::pair<int64_t, int64_t>(7, 25));

    for (auto &e : map)
        std::cout << e.first << ", " << e.second << std::endl;
        
     // 0, 5
     // 7, 25
     // -2, 5
     
     return 0;
}

```
## Custom types as keys
```C++
struct Employee {
    int id;
    std::string first_name;
    std::string last_name;

};

std::ostream &operator<<(std::ostream& os, const Employee &e) {
    os << e.first_name << " " << e.last_name << " (" << e.id << ")";
    return os;
}

namespace art {
    template<>
    struct key_transform<Employee> {
        int operator()(const Employee &e) const noexcept {
            return key_transform<int>()(e.id);
        }
    };
}
    
int main() {
    art::radix_map<Employee, int> employee_map = {
                {{1, "Alan", "Turing"},  10},
                {{-5, "John", "Neumann"}, 20}
        };
    
        for (auto &e : employee_map)
            std::cout << e.first << ", " << e.second << std::endl;
    
    // John Neumann (-5), 20        
    // Alan Turing (1), 10
    return 0;
}
```
