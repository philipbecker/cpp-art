//
// Created by philip on 06.07.16.
//

#ifndef ART_ADAPT_RADIX_TREE_H
#define ART_ADAPT_RADIX_TREE_H

#include <bits/unique_ptr.h>
#include "Node.h"
#include "Node256.h"

class adapt_radix_tree {
private:
    Node* root;

public:
    adapt_radix_tree() {
        this->root = nullptr;
    }

    void insert(const int &x);

    bool find(const int &x);

    void traverse();
};


#endif //ART_ADAPT_RADIX_TREE_H
