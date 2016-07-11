#include <iostream>
#include <bitset>
#include "adapt_radix_tree.h"
#include "_leaf.h"
#include "node_4.h"

std::pair<_node *, bool> adapt_radix_tree::insert(const int &x) {
    Key key = {x};
    Leaf *new_leaf = new Leaf(key);

    //std::cout << "INSERT(" << x << "): " << print_key(key) << std::endl;

    // Empty Tree
    if (root == nullptr) {
        root = new_leaf;
        count++;
        return std::make_pair(root, true);
    }

    // Replace Leaf with inner node
    /**
    if (root->is_leaf()) {
        Leaf *existing_leaf = reinterpret_cast<Leaf*>(root);
        Key existing_key = existing_leaf->get_key();
        root = new Node256(existing_leaf, 0);

        Node** current_node = &root;
        for (unsigned j = 0; j < sizeof(x); j++) {
            if (existing_key.chunks[j] == key.chunks[j]) {
                Node** old_child = (*current_node)->find(existing_key, j);
                Node* new_child = new Node256(existing_leaf, j+1);
                *old_child = new_child;
                current_node = old_child;
            } else {
                (*current_node)->insert(key.chunks[j], new_leaf);
                return;
            }
        }
        return;
    }
     */

    _node **current_node = &root;
    _node **previous_node = nullptr;

    for (unsigned i = 0; i < sizeof(x); i++) {
        if (*current_node != nullptr && (*current_node)->is_leaf()) {
            Leaf *existing_leaf = reinterpret_cast<Leaf *>(*current_node);
            if (existing_leaf->contains(key, i)) {
                return std::make_pair(existing_leaf, false);
            } else {
                Key existing_key = existing_leaf->get_key();
                *current_node = new Node256(existing_leaf, i);
                for (unsigned j = i; j < sizeof(x); j++) {
                    if (existing_key.chunks[j] == key.chunks[j]) {
                        Node **old_child = (*current_node)->find(existing_key, j);
                        Node *new_child = new Node256(existing_leaf, j + 1);
                        *old_child = new_child;
                        current_node = old_child;
                    } else {
                        (*current_node)->insert(key.chunks[j], new_leaf);
                        count++;
                        return std::make_pair(new_leaf, true);
                    }
                }
                count++;
                return std::make_pair(*current_node, true);
            }
        }
        else if (*current_node != nullptr) {
            previous_node = current_node;
            current_node = (*current_node)->find(key, i);
        } else {
            (*previous_node)->insert(key.chunks[i - 1], new_leaf);
            count++;
            return std::make_pair(new_leaf, true);
        }
    }
}

bool adapt_radix_tree::find(const int &x) {
    //std::cout << "FIND(" << x << ")" << std::endl;
    Key key = {x};

    if (root == nullptr)
        return false;

    _node *current_node = root;
    for (size_t i = 0; i < sizeof(x) + 1; i++) {
        if (current_node == nullptr)
            return false;
        if (current_node->is_leaf())
            return ((Leaf *) current_node)->contains(key, i);

        current_node = *current_node->find(key, i);
    }

    return false;
}

void adapt_radix_tree::traverse() {

}

Node *adapt_radix_tree::grow(Node *old_node) {

}



