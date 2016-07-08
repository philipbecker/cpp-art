#include <iostream>
#include "adapt_radix_tree.h"

using namespace std;

int main() {
    adapt_radix_tree art;
    art.insert(5);
    art.insert(6);
    art.insert(7);
    art.insert(260);
    art.insert(261);
    art.insert(262);

    cout << "TRAVERSE" << endl;
    art.traverse();


    for (auto &i : {1, 2, 5, 260, 262, 263, 261, 8, 7}) {
        cout << "Find(" << i << "): " << art.find(i) << endl;
    }
    return 0;
}