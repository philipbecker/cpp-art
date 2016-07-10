#include <iostream>
#include "adapt_radix_tree.h"

using namespace std;

int main() {
    art::adapt_radix_tree<int, int> art;
    art.insert(make_pair(5, 5));
    art.insert(make_pair(6, 1231));
//    art.insert(7);
//    art.insert(260);
//    art.insert(261);
//    art.insert(262);

    //cout << "TRAVERSE" << endl;
    //art.traverse();


    for (auto &i : {1, 2, 5, 260, 262, 263, 261, 8, 7}) {
        cout << "Find(" << i << "): " << art.find(i) << endl;
    }
    return 0;
}