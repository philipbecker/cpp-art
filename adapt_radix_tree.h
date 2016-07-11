#ifndef ART_ADAPT_RADIX_TREE_H
#define ART_ADAPT_RADIX_TREE_H

#include <bits/unique_ptr.h>
#include <utility>
#include "_node.h"
#include "node_256.h"

namespace art
{
    template<typename _Key, typename _Tp>
    class adapt_radix_tree {
    private:
        size_t _count;

        typedef _Key key_type;
        typedef _Tp mapped_type;
        typedef std::pair<const _Key, _Tp> value_type;

    public:
        _node *_root;

        adapt_radix_tree() : _count(0) {
            this->_root = nullptr;
        }

        std::pair<_node *, bool> insert(const value_type &x) {
            Key key = {x.first};
            _leaf *new_leaf = new _leaf(key);

            // Empty Tree
            if (_root == nullptr) {
                _root = new_leaf;
                _count++;
                return std::make_pair(_root, true);
            }

            _node **current_node = &_root;
            _node **previous_node = nullptr;

            for (unsigned i = 0; i < sizeof(x); i++) {
                if (current_node != nullptr && *current_node != nullptr && (*current_node)->is_leaf()) {
                    // Hit an existing leaf
                    _leaf *existing_leaf = reinterpret_cast<_leaf *>(*current_node);
                    if (existing_leaf->contains(key, i)) {
                        // if it is a duplicate entry, ignore
                        delete new_leaf;
                        return std::make_pair(existing_leaf, false);
                    } else {
                        // otherwise, the leaf needs to be replaced by a node 4
                        Key existing_key = existing_leaf->get_key();
                        *current_node = new node_4(existing_leaf, i);
                        // if the keys are matching, go down all the way until we find a tiebreaker
                        // insert node4's with one child all the way down until a final node 4 with 2 children
                        for (unsigned j = i; j < sizeof(x); j++) {
                            if (existing_key.chunks[j] == key.chunks[j]) {
                                _node **old_child = (*current_node)->find(existing_key.chunks[j]);
                                _node *new_child = new node_4(existing_leaf, j + 1);
                                *old_child = new_child;
                                current_node = old_child;
                            } else {
                                if ((*current_node)->size() == (*current_node)->max_size())
                                    *current_node = grow(*current_node);
                                (*current_node)->insert(key.chunks[j], new_leaf);
                                _count++;
                                return std::make_pair(new_leaf, true);
                            }
                        }
                        throw; // unreachable
                    }
                } else if (current_node != nullptr && *current_node != nullptr) {
                    // traverse down the tree
                    previous_node = current_node;
                    current_node = (*current_node)->find(key.chunks[i]);
                } else {
                    // hit empty point, this can only happen if the inserted key
                    // is not a prefix/equal to another key alread in the tree
                    // therefore we can just insert a new leaf
                    // previous node might have to be grown before that
                    if ((*previous_node)->size() == (*previous_node)->max_size())
                        *previous_node = grow(*previous_node);
                    (*previous_node)->insert(key.chunks[i - 1], new_leaf);
                    _count++;
                    return std::make_pair(new_leaf, true);
                }
            }
            throw; // unreachable
        }

        bool find(const key_type &x) {
            Key key = {x};

            if (_root == nullptr)
                return false;

            _node **current_node = &_root;
            for (size_t i = 0; i < sizeof(x) + 1; i++) {
                if (current_node == nullptr || *current_node == nullptr)
                    return false;
                if ((*current_node)->is_leaf())
                    return ((_leaf *) *current_node)->contains(key, i);

                current_node = (*current_node)->find(key.chunks[i]);
            }

            return false;
        }

        void traverse() {
            _root->traverse(0);
        }

        size_t size() const {
            return _count;
        }

    private:
        _node *grow(_node *old_node) {
            auto type = old_node->get_type();
            switch (type) {
                case node_type::node_4_t: {
                    node_4 *node4 = static_cast<node_4 *>(old_node);
                    _node *node = new node_16(node4);
                    return node;
                }
                case node_type::node_16_t: {
                    node_16 *node16 = static_cast<node_16 *>(old_node);
                    _node *node = new node_48(node16);
                    return node;
                }
                case node_type::node_48_t: {
                    node_48 *node48 = static_cast<node_48 *>(old_node);
                    _node *node = new node_256(node48);
                    return node;
                }
            }
            throw; // unreachable
        }

        _node *shrink(_node *old_node) {
            auto type = old_node->get_type();
            switch (type) {
                case node_type::node_4_t: {
                    node_4 *node4 = static_cast<node_4 *>(old_node);
                    _node *node = node4->children[0];
                    return node;
                }
                case node_type::node_16_t: {
                    node_16 *node16 = static_cast<node_16 *>(old_node);
                    _node *node = new node_4(node16);
                    return node;
                }
                case node_type::node_48_t: {
                    node_48 *node48 = static_cast<node48 *>(old_node);
                    _node *node = new node_16(node48);
                    return node;
                }
                case node_type::node_256_t: {
                    node_256 *node256 = static_cast<node_256 *>(old_node);
                    _node *node = new node_48(node256);
                    return node;
                }
            }
            throw; // unreachable
        }
    };
}


#endif //ART_ADAPT_RADIX_TREE_H
