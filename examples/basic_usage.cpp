#include <iostream>
#include <art/radix_map.h>

int main() {
    using std::cout;
    using std::endl;

    art::radix_map<int, std::string> m;

    // insert with pair, initializer list and emplace
    m.emplace(1, "This");
    m.insert({{3, "the"}, {2, "is"}});
    m.insert(std::pair<int, std::string>(4, "end"));
    m.emplace(5, "!!!");

    for (auto &e : m)
        cout << e.second << " ";
    cout << endl;

    // erase by iterator and key
    m.erase(4);
    m.erase(m.find(3));
    m.emplace(4, "SPARTA");

    for (auto &e : m)
        cout << e.second << " ";
    cout << endl;

    // move/copy construction and assignment
    art::radix_map<int, std::string> m2 = std::move(m);
    art::radix_map<int, std::string> m3(m2);

    // construction with iterator
    art::radix_map<int, std::string> m4(m2.begin(), m2.end());

    m.emplace(78, "source map for move operation is valid and empty");
    for (auto &e : m)
        cout << e.second << " ";
    cout << endl;

    for (auto &e : m2)
        cout << e.second << " ";
    cout << endl;

    // element acces with [] operator and at()
    m3[4] = "SPAAARTAAAAAAAAAAAAAAAAAAA";
    m3[8] = "???";

    try {
        m3.at(6) = "OUT OF BOUNDS";
    } catch (std::exception &e) {
        cout << e.what() << endl;
    }

    for (auto &e : m3)
        cout << e.second << " ";
    cout << endl;

    // relational operators
    cout << "m3 == m2: " << (m3 == m2) << endl;
    cout << "m3 < m2: " << (m3 < m2) << endl;

    art::swap(m2, m3);
    cout << "m3 < m2: " << (m3 < m2) << endl;

    // erasing ranges
    m2.erase(m2.find(2), m2.find(5));

    // reverse iteration
    for (auto it = m2.crbegin(), end = m2.crend(); it != end; it++)
        cout << it->second << " ";
    cout << endl;

    return 0;
}