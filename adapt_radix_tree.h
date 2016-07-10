#ifndef ART_ADAPT_RADIX_TREE_H
#define ART_ADAPT_RADIX_TREE_H

#include <bits/unique_ptr.h>
#include <utility>
#include "Node.h"
#include "Node256.h"

namespace art
{
    template<typename _Key, typename _Tp>
    class adapt_radix_tree {
    private:
        Node *root;
        size_t count;

        typedef _Key key_type;
        typedef _Tp mapped_type;
        typedef std::pair<const _Key, _Tp> value_type;

    public:
        adapt_radix_tree() : count(0) {
            this->root = nullptr;
        }

        std::pair<Node *, bool> insert(const value_type &x) {
            Key key = {x.first};
            Leaf *new_leaf = new Leaf(key);

            //std::cout << "INSERT(" << x << "): " << print_key(key) << std::endl;

            // Empty Tree
            if (root == nullptr) {
                root = new_leaf;
                count++;
                return std::make_pair(root, true);
            }

            Node **current_node = &root;
            Node **previous_node = nullptr;

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
                                Node **old_child = (*current_node)->find(existing_key.chunks[j]);
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
                    current_node = (*current_node)->find(key.chunks[i]);
                } else {
                    (*previous_node)->insert(key.chunks[i - 1], new_leaf);
                    count++;
                    return std::make_pair(new_leaf, true);
                }
            }
        };

        bool find(const key_type &x) {
            Key key = {x};

            if (root == nullptr)
                return false;

            Node *current_node = root;
            for (size_t i = 0; i < sizeof(x) + 1; i++) {
                if (current_node == nullptr)
                    return false;
                if (current_node->is_leaf())
                    return ((Leaf *) current_node)->contains(key, i);

                current_node = *current_node->find(key.chunks[i]);
            }

            return false;
        }

        void traverse() {
            root->traverse(0);
        }

        size_t size() const {
            return count;
        }

    private:
        Node *grow(Node *old_node) {
            /**
            auto type = old_node->get_type();
            switch (type) {
                case NodeType::Node4: {
                    Node4 *node4 = static_cast<Node4 *>(old_node);
                    Node16 *node = new Node16(node4);
                    return node;
                }
                case NodeType::Node16: {
                    Node16 *node16 = static_cast<Node16 *>(old_node);
                    Node48 *node = new Node48(node16);
                    return node;
                }
                case NodeType::Node48: {
                    Node48 *node48 = static_cast<Node48 *>(old_node);
                    Node256 *node = new Node256(node48);
                    return node;
                }
            }
            */
        }
    };
}


#endif //ART_ADAPT_RADIX_TREE_H
