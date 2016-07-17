#ifndef ART_ADAPT_RADIX_TREE_H
#define ART_ADAPT_RADIX_TREE_H

#include <stddef.h>
#include <assert.h>
#include <iterator>
#include <bits/unique_ptr.h>
#include <utility>
#include <bitset>
#include <iostream>
#include <stack>
#include "key_transform.h"

namespace art
{
    typedef uint8_t byte;
    static const uint8_t EMPTY_MARKER = 48;

    template<typename _Key, typename _Tp,
            typename _Key_transform = key_transform<_Key>,
            typename _Alloc = std::allocator<std::pair<const _Key, _Tp> > >
    class Adaptive_radix_tree {
    public:
        // Forward declaration for typedefs
        class _Node;

        class _Node_16;

        class _Node_48;

        class _Node_256;

        typedef _Key key_type;
        typedef _Tp mapped_type;
        typedef std::pair<const _Key, _Tp> value_type;

    private:
        enum node_type : uint8_t {
            _leaf_t = 0, node_4_t = 1, node_16_t = 2,
            node_48_t = 3, node_256_t = 4, _dummy_node = 5
        };

    public:
        union Key {
            const key_type value;
            const byte chunks[sizeof(key_type)];
        };

        class _Node {
        protected:
            uint16_t _count;

        public:
            _Node *_parent;

            _Node(uint16_t count, _Node *parent)
                    : _count(count), _parent(parent) { }

            virtual void clear() = 0;

            virtual void insert(const byte &key_byte, _Node *node) = 0;

            virtual _Node **find(const byte &key_byte) = 0;

            virtual _Node *minimum() = 0;

             virtual _Node *maximum() = 0;

            virtual void traverse(unsigned depth) = 0;

            virtual bool is_leaf() { return false; }

            uint16_t size() const { return _count; }

            virtual uint16_t max_size() const = 0;

            virtual node_type get_type() const = 0;

            /**
             * @brief  Find the successor of key stored at pos or later.
             *
             * @param key  The key we want to find the successor of.
             * @param depth   Current depth in the tree (in order to get the proper
             *                  key byte)
             * @return  A pair, of which the first element is a success flag, denoting
             *          whether a successor was found or not, and the second is a pointer
             *          to the node. In case of failure, a nullptr is returned as the second
             *          element.
             *
             * Finding a successor takes O(k) time.
             */
            virtual _Node *successor(const Key &key, int depth) = 0;

            /**
             * @brief  Find the predecessor of key stored at pos or earlier.
             *
             * @param key  The key we want to find the predecessor of.
             * @param depth   Current depth in the tree (in order to get the proper
             *                  key byte)
             * @return  A pair, of which the first element is a success flag, denoting
             *          whether a predecessor was found or not, and the second is a pointer
             *          to the node. In case of failure, a nullptr is returned as the second
             *          element.
             *
             * Finding a predecessor takes O(k) time.
             */
            virtual _Node *predecessor(const Key &key, int depth) = 0;
        };

        class _Leaf : public _Node {
        public:
            Key key;
            value_type value;
            int depth;

            _Leaf(Key key, int depth, _Node *parent)
                    : _Node(1, parent), key(key), depth(depth) {
            }

            _Leaf(Key key, value_type value, int depth, _Node *parent)
                    : _Node(1, parent), key(key), value(value), depth(depth) {
            }

            void clear() { }

            virtual void insert(const byte &key_byte, _Node *node) override { }

            virtual void traverse(unsigned depth) override {
                std::cout << std::string(depth + 1, '-') << " leaf " << this << ": ("
                << value.first << "," << value.second << ")" << std::endl;
            }

            virtual _Node **find(const byte &key_byte) override {
                return nullptr;
            }

            virtual _Node *minimum() override { return this; }

            virtual _Node *maximum() override { return this; }

            virtual _Node *successor(const Key &key, int depth) override {
                std::cout << "succesor leaf " << depth << ", value;" << value.second << std::endl;
                return this;
            }

            virtual _Node *predecessor(const Key &key, int depth) override {
                return this;
            }

            bool is_leaf() override { return true; }

            uint16_t max_size() const override { return 1; }

            virtual node_type get_type() const override { return node_type::_leaf_t; }
        };

        /**
         * End of iteration marker right of the container.
         */
        class _Dummy_Node : public _Node {
        public:
            _Node **_root;

            _Leaf *_leaf;

            _Dummy_Node()
                    : _Node(0, nullptr), _root(nullptr),
                      _leaf(new _Leaf(Key{0}, std::pair<key_type, mapped_type>(0, 0), 0, this)) {
            }

            _Dummy_Node(_Node **root, _Leaf *leaf)
                    : _Node(0, nullptr), _root(root),
                      _leaf(new _Leaf(leaf->key, leaf->value, 0, this)) {
            }

            void clear() { }

            virtual void insert(const byte &key_byte, _Node *node) override { }

            virtual void traverse(unsigned depth) override { }

            virtual _Node **find(const byte &key_byte) override {
                return nullptr;
            }

            virtual _Node *minimum() override { return this; }

            virtual _Node *maximum() override { return this; }

            virtual _Node *successor(const Key &key, int depth) override {
                return this->_leaf;
            }

            virtual _Node *predecessor(const Key &key, int depth) override {
                if (_root != nullptr)
                    return (*_root)->maximum();

                return this->_leaf;
            }

            uint16_t max_size() const override { return 1; }

            virtual node_type get_type() const override { return node_type::_dummy_node; }
        };

        class _Node_4 : public _Node {
        public:
            std::array<byte, 4> keys{};
            std::array<_Node *, 4> children{};

            _Node_4(_Leaf *leaf, int depth)
                    : _Node(1, leaf->_parent) {
                Key key = leaf->key;
                leaf->depth = depth + 1;
                keys[0] = key.chunks[depth];
                children[0] = leaf;

                children[0]->_parent = this;
            }

            _Node_4(_Node_16 *node) : _Node(4, node->_parent) {
                std::copy(node->keys.begin(), node->keys.begin() + 4, keys.begin());
                std::copy(node->children.begin(), node->children.begin() + 4, children.begin());

                for (size_t i = 0; i < 4; i++)
                    children[i]->_parent = this;

                delete node;
            }

            void clear() {
                for (uint16_t i = 0; i < this->_count; i++) {
                    children[i]->clear();
                    delete children[i];
                }
            }

            virtual void insert(const byte &key_byte, _Node *node) override {
                unsigned pos = 0;
                for (; pos < this->_count && keys[pos] < key_byte; pos++);
                if (pos < this->_count) {
                    std::move(keys.begin() + pos, keys.begin() + this->_count, keys.begin() + pos + 1);
                    std::move(children.begin() + pos, children.begin() + this->_count, children.begin() + pos + 1);
                }
                keys[pos] = key_byte;
                children[pos] = node;
                this->_count++;
            }

            virtual _Node **find(const byte &key_byte) override {
                unsigned pos = 0;
                for (; pos < this->_count && keys[pos] < key_byte; pos++);

                if (pos < this->_count && keys[pos] == key_byte)
                    return &children[pos];
                return nullptr;
            }

            virtual _Node *minimum() override {
                return children[0]->minimum();
            }

            virtual _Node *maximum() override {
                return children[this->_count - 1]->maximum();
            }

            virtual void traverse(unsigned depth) override {
                for (size_t i = 0; i < this->_count; i++)
                    children[i]->traverse(depth + 1);
            }

            virtual _Node *successor(const Key &key, int depth) override {
                unsigned pos = 0;
                for (; pos < this->_count && keys[pos] < key.chunks[depth]; pos++);
                if (pos + 1 < this->_count && keys[pos] == key.chunks[depth])
                    return children[pos + 1]->minimum();
                else
                    return this->_parent->successor(key, depth - 1);
            }

            virtual _Node *predecessor(const Key &key, int depth) override {
                int pos = this->_count - 1;
                for (; pos > 0 && keys[pos] > key.chunks[depth]; pos--);
                if (pos - 1 >= 0 && keys[pos] == key.chunks[depth])
                    return children[pos - 1]->maximum();
                else
                    return this->_parent->predecessor(key, depth - 1);
            }

            virtual uint16_t max_size() const override { return 4; }

            virtual node_type get_type() const override { return node_type::node_4_t; }

        };

        class _Node_16 : public _Node {
        public:
            std::array<byte, 16> keys{};
            std::array<_Node *, 16> children{};

            _Node_16(_Node_4 *node) : _Node(4, node->_parent) {
                assert(node->size() == 4);

                std::copy(node->keys.begin(), node->keys.end(), keys.begin());
                std::copy(node->children.begin(), node->children.end(), children.begin());

                for (size_t i = 0; i < 4; i++)
                    children[i]->_parent = this;

                delete node;
            }

            _Node_16(_Node_48 *node) : _Node(16, node->_parent) {
                assert(node->size() == 16);

                uint8_t pos = 0;
                for (uint16_t i = 0; i < 256; i++) {
                    if (node->child_index[i] != EMPTY_MARKER) {
                        keys[pos] = i;
                        children[pos] = node->children[node->child_index[i]];
                        children[pos]->_parent = this;
                        pos++;
                    }
                }
                delete node;
            }

            void clear() {
                for (uint16_t i = 0; i < this->_count; i++) {
                    children[i]->clear();
                    delete children[i];
                }
            }

            virtual void insert(const byte &key_byte, _Node *node) override {
                unsigned pos = 0;
                for (; pos < this->_count && keys[pos] < key_byte; pos++);
                if (pos != this->_count) {
                    std::move(keys.begin() + pos, keys.begin() + this->_count, keys.begin() + pos + 1);
                    std::move(children.begin() + pos, children.begin() + this->_count, children.begin() + pos + 1);
                }
                keys[pos] = key_byte;
                children[pos] = node;
                this->_count++;
            }

            virtual _Node **find(const byte &key_byte) override {
                unsigned pos = 0;
                for (; pos < this->_count && keys[pos] < key_byte; pos++);

                if (pos < this->_count && keys[pos] == key_byte)
                    return &children[pos];
                return nullptr;
            }

            virtual void traverse(unsigned depth) override {
                for (uint8_t i = 0; i < this->_count; i++)
                    children[i]->traverse(depth + 1);
            }

            virtual _Node *minimum() override {
                return children[0]->minimum();
            }

            virtual _Node *maximum() override {
                return children[this->_count - 1]->maximum();
            }

            virtual _Node *successor(const Key &key, int depth) override {
                unsigned pos = 0;
                for (; pos < this->_count && keys[pos] < key.chunks[depth]; pos++);
                if (pos + 1 < this->_count && keys[pos] == key.chunks[depth])
                    return children[pos + 1]->minimum();
                else
                    return this->_parent->successor(key, depth - 1);
            }

            virtual _Node *predecessor(const Key &key, int depth) override {
                int pos = this->_count - 1;
                for (; pos > 0 && keys[pos] > key.chunks[depth]; pos--);
                if (pos - 1 >= 0 && keys[pos] == key.chunks[depth])
                    return children[pos - 1]->maximum();
                else
                    return this->_parent->predecessor(key, depth - 1);
            }

            virtual uint16_t max_size() const override { return 16; }

            virtual node_type get_type() const override { return node_type::node_16_t; }
        };


        class _Node_48 : public _Node {
        public:
            std::array<byte, 256> child_index;
            std::array<_Node *, 48> children{};

            _Node_48(_Node_16 *node) : _Node(16, node->_parent) {
                assert(node->size() == 16);

                std::fill(child_index.begin(), child_index.end(), EMPTY_MARKER);

                for (uint8_t i = 0; i < 16; i++) {
                    child_index[node->keys[i]] = i;
                    children[i] = node->children[i];
                    children[i]->_parent = this;
                }

                delete node;
            }

            _Node_48(_Node_256 *node) : _Node(48, node->_parent) {
                assert(node->size() == 48);

                std::fill(child_index.begin(), child_index.end(), EMPTY_MARKER);

                uint8_t pos = 0;
                for (uint16_t i = 0; i < 256; i++) {
                    if (node->children[i] != nullptr) {
                        child_index[i] = pos;
                        children[pos] = node->children[i];
                        children[pos]->_parent = this;
                        pos++;
                    }
                }

                delete node;
            }

            void clear() {
                for (size_t i = 0; i < 256; i++) {
                    if (child_index[i] != EMPTY_MARKER) {
                        children[child_index[i]]->clear();
                        delete children[child_index[i]];
                    }
                }
            }

            virtual void insert(const byte &key_byte, _Node *node) override {
                auto pos = this->_count;
                child_index[key_byte] = pos;
                children[pos] = node;
                this->_count++;
            }

            virtual _Node **find(const byte &key_byte) override {
                if (child_index[key_byte] != EMPTY_MARKER)
                    return &children[child_index[key_byte]];
                return nullptr;
            }

            virtual _Node *minimum() override {
                unsigned pos = 0;
                while (child_index[pos] == EMPTY_MARKER)
                    pos++;
                return children[child_index[pos]]->minimum();
            }

            virtual _Node *maximum() override {
                auto pos = 255;
                while (child_index[pos] == EMPTY_MARKER)
                    pos--;
                return children[child_index[pos]]->maximum();
            }

            virtual void traverse(unsigned depth) override {
                for (size_t i = 0; i < 256; i++)
                    if (child_index[i] != EMPTY_MARKER)
                        children[child_index[i]]->traverse(depth + 1);
            }

            virtual _Node *successor(const Key &key, int depth) override {
                for (unsigned pos = ((unsigned) key.chunks[depth]) + 1; pos < 256; pos++)
                    if (child_index[pos] != EMPTY_MARKER)
                        return children[child_index[pos]]->minimum();

                return this->_parent->successor(key, depth - 1);
            }

            virtual _Node *predecessor(const Key &key, int depth) override {
                for (int pos = ((int) key.chunks[depth]) - 1; pos >= 0; pos--)
                    if (child_index[pos] != EMPTY_MARKER)
                        return children[child_index[pos]]->maximum();

                return this->_parent->predecessor(key, depth - 1);
            }

            virtual uint16_t max_size() const override { return 48; }

            virtual node_type get_type() const override { return node_type::node_48_t; }
        };

        class _Node_256 : public _Node {
        public:
            std::array<_Node *, 256> children{};

            _Node_256(_Node_48 *node) : _Node(48, node->_parent) {
                assert(node->size() == 48);

                for (uint16_t i = 0; i < 256; i++) {
                    if (node->child_index[i] != EMPTY_MARKER) {
                        children[i] = node->children[node->child_index[i]];
                        children[i]->_parent = this;
                    }
                }

                delete node;
            }

            void clear() {
                for (size_t i = 0; i < 256; i++) {
                    if (children[i] != nullptr) {
                        children[i]->clear();
                        delete children[i];
                    }
                }
            }

            virtual void insert(const byte &key_byte, _Node *node) override {
                children[key_byte] = node;
                this->_count++;
            }

            virtual _Node **find(const byte &key_byte) override {
                if (children[key_byte] != nullptr)
                    return &children[key_byte];
                return nullptr;
            }

            virtual void traverse(unsigned depth) override {
                std::cout << std::string(depth + 1, '-') << " _node " << this << std::endl;
                for (int i = 0; i < 256; i++) {
                    if (children[i] != nullptr) {

                        std::bitset<8> x(i);
                        std::cout << std::string(depth + 1, '-') << " Go along key: " << x << std::endl;

                        children[i]->traverse(depth + 1);
                    }
                }
            }

            virtual _Node *minimum() override {
                unsigned pos = 0;
                while (children[pos] == nullptr)
                    pos++;
                return children[pos]->minimum();
            }

            virtual _Node *maximum() override {
                auto pos = 255;
                while (children[pos] == nullptr)
                    pos--;
                return children[pos]->maximum();
            }

            virtual _Node *successor(const Key &key, int depth) override {
                for (unsigned pos = ((unsigned) key.chunks[depth]) + 1; pos < 256; pos++)
                    if (children[pos] != nullptr)
                        return children[pos]->minimum();

                return this->_parent->successor(key, depth - 1);
            }

            virtual _Node *predecessor(const Key &key, int depth) override {
                for (int pos = key.chunks[depth] - 1; pos >= 0; pos--)
                    if (children[pos] != nullptr)
                        return children[pos]->maximum();

                return this->_parent->predecessor(key, depth - 1);
            }

            virtual uint16_t max_size() const override { return 256; }

            virtual node_type get_type() const override { return node_type::node_256_t; }

        };

    private:
        size_t _M_count;

        _Key_transform _M_key_transform;

        // Dummy node of special type to mark the right end of the container
        // Necessary as the end marker for iterators
        _Dummy_Node *_M_dummy_node;

    public:
        // Current root node of the radix tree
        _Node *_M_root;

        Adaptive_radix_tree() : _M_count(0) {
            _M_root = nullptr;
            _M_dummy_node = new _Dummy_Node();
        }

        Adaptive_radix_tree(const _Key_transform &key_transformer)
                : _M_count(0), _M_key_transform(key_transformer) {
            _M_root = nullptr;
            _M_dummy_node = new _Dummy_Node();
        }

        //////////////
        // Capacity //
        //////////////

        size_t size() const {
            return _M_count;
        }

        bool empty() const {
            return _M_count == 0;
        }

        ///////////////////
        // Debug helpers //
        ///////////////////

        static std::string print_key(Key key, unsigned depth) {
            std::bitset<8> x(key.chunks[depth]);
            std::string output = x.to_string();
            return output;
        }

        static std::string print_key(Key key) {
            std::string output;
            for (unsigned i = 0; i < sizeof(key.value); i++) {
                output += print_key(key, i) + ", ";
            }
            return output;
        }

        ///////////////
        // Iterators //
        ///////////////

        struct adapt_radix_tree_iterator {
            typedef std::pair<const _Key, _Tp> value_type;
            typedef value_type &reference;
            typedef value_type *pointer;

            typedef std::bidirectional_iterator_tag iterator_category;
            typedef ptrdiff_t difference_type;

            typedef adapt_radix_tree_iterator _Self;
            typedef _Node *_Base_ptr;
            typedef _Leaf *_Link_type;

            // pointer to current leaf node
            _Node *node;

            adapt_radix_tree_iterator() : node(nullptr) { }


            adapt_radix_tree_iterator(_Node *node)
                    : node(node) {
            }

            // preincrement
            _Self &operator++() {
                Key key = static_cast<_Leaf *>(node)->key;
                int depth = static_cast<_Leaf *>(node)->depth;
                node = node->_parent->successor(key, depth - 1);
                return *this;
            }

            // postincrement
            _Self operator++(int) {
                _Self retval = *this;
                ++(*this);
                return retval;
            }

            // predecrement
            _Self &operator--() {
                Key key = static_cast<_Leaf *>(node)->key;
                int depth = static_cast<_Leaf *>(node)->depth;
                node = node->_parent->predecessor(key, depth - 1);
                return *this;
            }

            // postdecrement
            _Self operator--(int) {
                _Self retval = *this;
                --(*this);
                return retval;
            }

            bool operator==(const _Self &other) const {
                return node == other.node;
            }

            bool operator!=(const _Self &other) const {
                return !(*this == other);
            }

            reference operator*() const {
                return static_cast<_Link_type>(node)->value;
            }

            pointer operator->() const {
                return &static_cast<_Link_type>(node)->value;
            }
        };

        struct adapt_radix_tree_const_iterator {
            typedef std::pair<const _Key, _Tp> value_type;
            typedef const value_type &reference;
            typedef const value_type *pointer;

            typedef adapt_radix_tree_iterator iterator;

            typedef std::bidirectional_iterator_tag iterator_category;
            typedef ptrdiff_t difference_type;

            typedef adapt_radix_tree_const_iterator _Self;
            typedef _Node *_Base_ptr;
            typedef const _Leaf *_Link_type;

            // pointer to current leaf node
            _Node *node;

            adapt_radix_tree_const_iterator() : node(nullptr) { }

            // Copy Constructor
            adapt_radix_tree_const_iterator(const iterator &__it)
                    : node(__it.node) { }

            explicit adapt_radix_tree_const_iterator(_Node *node)
                    : node(node) {
            }


            // preincrement
            _Self &operator++() {
                Key key = static_cast<_Leaf *>(node)->key;
                int depth = static_cast<_Leaf *>(node)->depth;
                node = node->_parent->successor(key, depth - 1);
                return *this;
            }

            // postincrement
            _Self operator++(int) {
                _Self retval = *this;
                ++(*this);
                return retval;
            }

            // predecrement
            _Self &operator--() {
                Key key = static_cast<_Leaf *>(node)->key;
                int depth = static_cast<_Leaf *>(node)->depth;
                node = node->_parent->predecessor(key, depth - 1);
                return *this;
            }

            // postdecrement
            _Self operator--(int) {
                _Self retval = *this;
                --(*this);
                return retval;
            }

            bool operator==(const _Self &other) const {
                return node == other.node;
            }

            bool operator!=(const _Self &other) const {
                return !(*this == other);
            }

            reference operator*() const {
                return static_cast<_Link_type>(node)->value;
            }

            pointer operator->() const {
                return &static_cast<_Link_type>(node)->value;
            }
        };

        // @TODO const iterators
        typedef adapt_radix_tree_iterator iterator;
        typedef adapt_radix_tree_const_iterator const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        iterator begin() {
            _Node *min = minimum();
            if (min == nullptr)
                return iterator(_M_dummy_node->_leaf);

            return iterator(min);
        }

        const_iterator begin() const {
            _Node *min = minimum();
            if (min == nullptr)
                return const_iterator(_M_dummy_node->_leaf);
            return const_iterator(min);
        }

        iterator end() {
            return iterator(_M_dummy_node->_leaf);
        }

        const_iterator end() const {
            return const_iterator(_M_dummy_node->_leaf);
        }

        reverse_iterator rbegin() {
            return reverse_iterator(end());
        }

        const_reverse_iterator rbegin() const {
            return const_reverse_iterator(end());
        }

        reverse_iterator rend() {
            _Node *min = minimum();
            if (min == nullptr)
                return reverse_iterator(iterator(_M_dummy_node->_leaf));
            return reverse_iterator(iterator(min));
        }

        const_reverse_iterator rend() const {
            _Node *min = minimum();
            if (min == nullptr)
                return const_reverse_iterator(iterator(_M_dummy_node->_leaf));
            return const_reverse_iterator(iterator(min));
        }

        ///////////////
        // Modifiers //
        ///////////////

        // @TODO
        void clear() {
            if (_M_root != nullptr) {
                _M_root->clear();
                delete _M_root;
            }
        }

        // @TODO memory management?
        void _M_reset() {
            this->_M_root = nullptr;
            this->_M_count = 0;
        }

        std::pair<iterator, bool> _M_insert_unique(const value_type &__x) {
            Key transformed_key = {_M_key_transform(__x.first)};
            const auto key_size = sizeof(__x.first);

            // Empty Tree
            if (_M_root == nullptr) {
                _Leaf *new_leaf = new _Leaf(transformed_key, __x, 0, _M_dummy_node);
                _M_root = new_leaf;
                _M_count++;

                _M_dummy_node->_root = &_M_root;
                return std::make_pair(iterator(_M_root), true);
            }

            _Node **current_node = &_M_root;
            _Node **previous_node = nullptr;

            for (unsigned i = 0; i < key_size + 1; i++) {
                if (current_node != nullptr && *current_node != nullptr && (*current_node)->is_leaf()) {
                    // Hit an existing leaf
                    _Leaf *existing_leaf = reinterpret_cast<_Leaf *>(*current_node);
                    if (transformed_key.value == existing_leaf->key.value) {
                        // if it is a duplicate entry, ignore
                        return std::make_pair(iterator(existing_leaf), false);
                    } else {
                        // otherwise, the leaf needs to be replaced by a node 4
                        Key existing_key = existing_leaf->key;
                        *current_node = new _Node_4(existing_leaf, i);
                        // if the keys are matching, go down all the way until we find a tiebreaker
                        // insert node4's with one child all the way down until a final node 4 with 2 children
                        for (unsigned j = i; j < key_size + 1; j++) {
                            if (existing_key.chunks[j] == transformed_key.chunks[j]) {
                                _Node **old_child;
                                old_child = (*current_node)->find(existing_key.chunks[j]);
                                _Node *new_child = new _Node_4(existing_leaf, j + 1);
                                *old_child = new_child;
                                current_node = old_child;
                            } else {
                                if ((*current_node)->size() == (*current_node)->max_size())
                                    *current_node = grow(*current_node);

                                _Leaf *new_leaf = new _Leaf(transformed_key, __x, j + 1, *current_node);
                                (*current_node)->insert(transformed_key.chunks[j], new_leaf);
                                _M_count++;
                                return std::make_pair(iterator(new_leaf), true);
                            }
                        }
                        throw; // unreachable
                    }
                } else if (current_node != nullptr && *current_node != nullptr) {
                    // traverse down the tree
                    previous_node = current_node;
                    current_node = (*current_node)->find(transformed_key.chunks[i]);
                } else {
                    // hit empty point, this can only happen if the inserted key
                    // is not a prefix/equal to another key already in the tree
                    // therefore we can just insert a new leaf
                    // previous node might have to be grown before that
                    if ((*previous_node)->size() == (*previous_node)->max_size())
                        *previous_node = grow(*previous_node);

                    _Leaf *new_leaf = new _Leaf(transformed_key, __x, i, *previous_node);
                    (*previous_node)->insert(transformed_key.chunks[i - 1], new_leaf);
                    _M_count++;
                    return std::make_pair(iterator(new_leaf), true);
                }
            }
            throw; // unreachable
        }

        template<typename _InputIterator>
        void _M_insert_unique(_InputIterator __first, _InputIterator __last) {
            for (; __first != __last; ++__first)
                _M_insert_unique(*__first);
        }

        template<typename _InputIterator>
        void _M_assign_unique(_InputIterator __first, _InputIterator __last) {
            _M_reset();
            for (; __first != __last; ++__first)
                _M_insert_unique(*__first);
        }

        // @TODO implementation
        void swap(Adaptive_radix_tree &__x) {
            throw;
        }

        ////////////
        // Lookup //
        ////////////

        iterator find(const key_type &__k) {
            Key transformed_key = {_M_key_transform(__k)};
            const auto key_size = sizeof(__k);

            if (_M_root == nullptr)
                return end();

            _Node **current_node = &_M_root;
            for (unsigned i = 0; i < key_size + 1; i++) {
                if (current_node == nullptr || *current_node == nullptr)
                    return end();

                if ((*current_node)->is_leaf()) {
                    if (transformed_key.value == ((_Leaf *) *current_node)->key.value) {
                        return iterator(*current_node);
                    } else {
                        return end();
                    }
                }

                current_node = (*current_node)->find(transformed_key.chunks[i]);
            }
            throw; // unreachable
        }

        // @TODO test this function
        iterator lower_bound(const key_type &__k) {
            Key transformed_key = {_M_key_transform(__k)};
            const auto key_size = sizeof(__k);

            if (_M_root == nullptr)
                return end();

            _Node **previous_node = nullptr;
            _Node **current_node = &_M_root;
            unsigned pos = 0;
            for (unsigned i = 0; i < key_size + 1; i++) {
                if (current_node == nullptr || *current_node == nullptr) {
                    auto successor = (*previous_node)->successor(transformed_key, i - 1);
                    return iterator(successor);
                }

                if ((*current_node)->is_leaf()) {
                    if (((_Leaf *) *current_node)->key.value <= transformed_key.value) {
                        return iterator(*current_node);
                    } else {
                        auto successor = (*previous_node)->successor(transformed_key, i - 1);
                        return iterator(successor);
                    }
                }

                previous_node = current_node;
                current_node = (*current_node)->find(transformed_key.chunks[i]);
            }
            throw; // unreachable
        }

        iterator upper_bound(const key_type &__k) {
            throw;
        }

        _Node *minimum() const {
            if (_M_root != nullptr)
                return _M_root->minimum();
            return nullptr;
        }

        _Node *maximum() const {
            if (_M_root != nullptr)
                return _M_root->maximum();
            return nullptr;
        }

        void traverse() const {
            if (_M_root != nullptr)
                _M_root->traverse(0);
        }

        ~Adaptive_radix_tree() {
            clear();
            delete _M_dummy_node;
        }

    private:
        _Node *grow(_Node *old_node) {
            auto type = old_node->get_type();
            switch (type) {
                case node_type::node_4_t: {
                    _Node_4 *node4 = static_cast<_Node_4 *>(old_node);
                    _Node *node = new _Node_16(node4);
                    return node;
                }
                case node_type::node_16_t: {
                    _Node_16 *node16 = static_cast<_Node_16 *>(old_node);
                    _Node *node = new _Node_48(node16);
                    return node;
                }
                case node_type::node_48_t: {
                    _Node_48 *node48 = static_cast<_Node_48 *>(old_node);
                    _Node *node = new _Node_256(node48);
                    return node;
                }
            }
            throw; // unreachable
        }

        _Node *shrink(_Node *old_node) {
            auto type = old_node->get_type();
            switch (type) {
                case node_type::node_4_t: {
                    _Node_4 *node4 = static_cast<_Node_4 *>(old_node);
                    _Node *node = node4->children[0];
                    return node;
                }
                case node_type::node_16_t: {
                    _Node_16 *node16 = static_cast<_Node_16 *>(old_node);
                    _Node *node = new _Node_4(node16);
                    return node;
                }
                case node_type::node_48_t: {
                    _Node_48 *node48 = static_cast<_Node_48 *>(old_node);
                    _Node *node = new _Node_16(node48);
                    return node;
                }
                case node_type::node_256_t: {
                    _Node_256 *node256 = static_cast<_Node_256 *>(old_node);
                    _Node *node = new _Node_48(node256);
                    return node;
                }
            }
            throw; // unreachable
        }
    };

    //////////////////////////
    // Relational Operators //
    //////////////////////////

    template<typename _Key, typename _Tp, typename _Key_transform>
    inline bool
    operator==(const Adaptive_radix_tree<_Key, _Tp, _Key_transform> &lhs,
               const Adaptive_radix_tree<_Key, _Tp, _Key_transform> &rhs) {
        return lhs.size() == rhs.size()
               && std::equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    // Based on operator==
    template<typename _Key, typename _Tp, typename _Key_transform>
    inline bool
    operator!=(const Adaptive_radix_tree<_Key, _Tp, _Key_transform> &lhs,
               const Adaptive_radix_tree<_Key, _Tp, _Key_transform> &rhs) {
        return !(lhs == rhs);
    }

    template<typename _Key, typename _Tp, typename _Key_transform>
    inline bool
    operator<(const Adaptive_radix_tree<_Key, _Tp, _Key_transform> &__x,
              const Adaptive_radix_tree<_Key, _Tp, _Key_transform> &__y) {
        return std::lexicographical_compare(__x.begin(), __x.end(),
                                            __y.begin(), __y.end());
    }

    // Based on operator<
    template<typename _Key, typename _Tp, typename _Key_transform>
    inline bool
    operator>(const Adaptive_radix_tree<_Key, _Tp, _Key_transform> &__x,
              const Adaptive_radix_tree<_Key, _Tp, _Key_transform> &__y) {
        return __y < __x;
    }

    // Based on operator<
    template<typename _Key, typename _Tp, typename _Key_transform>
    inline bool
    operator<=(const Adaptive_radix_tree<_Key, _Tp, _Key_transform> &__x,
               const Adaptive_radix_tree<_Key, _Tp, _Key_transform> &__y) {
        return !(__y < __x);
    }

    // Based on operator<
    template<typename _Key, typename _Tp, typename _Key_transform>
    inline bool
    operator>=(const Adaptive_radix_tree<_Key, _Tp, _Key_transform> &__x,
               const Adaptive_radix_tree<_Key, _Tp, _Key_transform> &__y) {
        return !(__x < __y);
    }
}


#endif //ART_ADAPT_RADIX_TREE_H
