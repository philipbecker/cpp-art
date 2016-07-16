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
        typedef std::stack<std::pair<_Node *, unsigned>> parent_iter_stack;

    private:
        size_t _M_count;
        _Key_transform _M_key_transform;

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
            _Node(uint16_t count) : _count(count) { }

            virtual unsigned insert(const byte &key_byte, _Node *node) = 0;

            virtual std::pair<_Node **, unsigned> find(const byte &key_byte) = 0;

            virtual _Node *minimum() = 0;

            virtual _Node *minimum(parent_iter_stack &parents) = 0;

            virtual _Node *maximum() = 0;

            virtual _Node *maximum(parent_iter_stack &parents) = 0;

            virtual void traverse(unsigned depth) = 0;

            virtual bool is_leaf() { return false; }

            uint16_t size() const { return _count; }

            virtual uint16_t max_size() const = 0;

            virtual node_type get_type() const = 0;


            /**
             * @brief  Find the successor of key stored at pos or later.
             *
             * @param pos  Position from where the search should start.
             * @param parents   Pair of node's and positions. In case that there
             *                  are no more children after pos in the current node,
             *                  we go back up the tree and search from pos on.
             * @return  A pair, of which the first element is a success flag, denoting
             *          whether a successor was found or not, and the second is a pointer
             *          to the node. In case of failure, a nullptr is returned as the second
             *          element.
             *
             * Finding a successor takes O(k) time.
             */
            virtual std::pair<bool, _Node *> successor(unsigned pos, parent_iter_stack &parents) = 0;

            /**
             * @brief  Find the predecessor of key stored at pos or earlier.
             *
             * @param pos  Position from where the search should start.
             * @param parents   Pair of node's and positions. In case that there
             *                  are no more children before pos in the current node,
             *                  we go back up the tree and recurse.
             * @return  A pair, of which the first element is a success flag, denoting
             *          whether a predecessor was found or not, and the second is a pointer
             *          to the node. In case of failure, a nullptr is returned as the second
             *          element.
             *
             * Finding a predecessor takes O(k) time.
             */
            virtual std::pair<bool, _Node *> predecessor(int pos, parent_iter_stack &parents) = 0;

            /**
             * Stupid helper for reverse iteration.
             * @TODO replace this by something better
             */
            virtual uint16_t key_array_end() const = 0;
        };

        class _Leaf : public _Node {
        public:
            Key key;
            value_type value;

            _Leaf(Key key)
                    : _Node(1), key(key) {
            }

            _Leaf(Key key, value_type value)
                    : _Node(1), key(key), value(value) {
            }

            virtual unsigned insert(const byte &key_byte, _Node *node) override {
                return 0;
            }

            virtual void traverse(unsigned depth) override {
                std::cout << std::string(depth + 1, '-') << " leaf " << this << ": ("
                << value.first << "," << value.second << ")" << std::endl;
            }

            virtual std::pair<_Node **, unsigned> find(const byte &key_byte) override {
                return std::make_pair<_Node **, unsigned>(nullptr, 0);
            }

            virtual _Node *minimum() override { return this; }

            virtual _Node *minimum(parent_iter_stack &parents) override { return this; }

            virtual _Node *maximum() override { return this; }

            virtual _Node *maximum(parent_iter_stack &parents) override { return this; }

            virtual std::pair<bool, _Node *> successor(unsigned pos, parent_iter_stack &parents) override {
                return std::pair<bool, _Node *>(true, this);
            }

            virtual std::pair<bool, _Node *> predecessor(int pos, parent_iter_stack &parents) override {
                return std::pair<bool, _Node *>(true, this);
            }

            bool is_leaf() override { return true; }

            uint16_t max_size() const override { return 1; }

            virtual uint16_t key_array_end() const override { return 1; }

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
                    : _Node(0), _root(nullptr),
                      _leaf(new _Leaf(Key{0}, std::pair<key_type, mapped_type>(0, 0))) {
            }

            _Dummy_Node(_Node **root, _Leaf *leaf)
                    : _Node(0), _root(root),
                      _leaf(new _Leaf(leaf->key, leaf->value)) {
            }

            virtual unsigned insert(const byte &key_byte, _Node *node) override {
                return 0;
            }

            virtual void traverse(unsigned depth) override { }

            virtual std::pair<_Node **, unsigned> find(const byte &key_byte) override {
                return std::pair<_Node **, unsigned>(nullptr, 0);
            }

            virtual _Node *minimum() override { return this; }

            virtual _Node *minimum(parent_iter_stack &parents) override { return this; }

            virtual _Node *maximum() override { return this; }

            virtual _Node *maximum(parent_iter_stack &parents) override { return this; }

            virtual std::pair<bool, _Node *> successor(unsigned pos, parent_iter_stack &parents) override {
                return std::pair<bool, _Node *>(true, this->_leaf);
            }

            virtual std::pair<bool, _Node *> predecessor(int pos, parent_iter_stack &parents) override {
                if (_root != nullptr) {
                    _Node *max = (*_root)->maximum(parents);
                    return std::pair<bool, _Node *>(true, max);
                }
                return std::pair<bool, _Node *>(false, this->_leaf);
            }

            uint16_t max_size() const override { return 1; }

            virtual uint16_t key_array_end() const override { return 1; }

            virtual node_type get_type() const override { return node_type::_dummy_node; }
        };

        class _Node_4 : public _Node {
        public:
            std::array<byte, 4> keys{};
            std::array<_Node *, 4> children{};

            _Node_4(_Leaf *leaf, unsigned depth) : _Node(1) {
                Key key = leaf->key;
                keys[0] = key.chunks[depth];
                children[0] = leaf;

            }

            _Node_4(_Node_16 *node) : _Node(4) {
                std::copy(node->keys.begin(), node->keys.begin() + 4, keys.begin());
                std::copy(node->children.begin(), node->children.begin() + 4, children.begin());
                delete node;
            }

            virtual unsigned insert(const byte &key_byte, _Node *node) override {
                unsigned pos = 0;
                for (; pos < this->_count && keys[pos] < key_byte; pos++) {

                }
                if (pos != this->_count) {
                    std::move(keys.begin() + pos, keys.begin() + this->_count, keys.begin() + pos + 1);
                    std::move(children.begin() + pos, children.begin() + this->_count, children.begin() + pos + 1);
                }
                keys[pos] = key_byte;
                children[pos] = node;
                this->_count++;
                return pos;
            }

            virtual std::pair<_Node **, unsigned> find(const byte &key_byte) override {
                unsigned pos = 0;
                for (; pos < this->_count && keys[pos] < key_byte; pos++);

                if (keys[pos] == key_byte)
                    return std::pair<_Node **, unsigned>(&children[pos], pos);
                return std::pair<_Node **, unsigned>(nullptr, 0);
            }

            virtual _Node *minimum() override {
                return children[0]->minimum();
            }

            virtual _Node *minimum(parent_iter_stack &parents) override {
                parents.push(std::make_pair(this, 0));
                return children[0]->minimum(parents);
            }

            virtual _Node *maximum() override {
                return children[this->_count - 1]->maximum();
            }

            virtual _Node *maximum(parent_iter_stack &parents) override {
                auto max = this->_count - 1;
                parents.push(std::make_pair(this, max));
                return children[max]->maximum(parents);
            }

            virtual void traverse(unsigned depth) override {
                for (size_t i = 0; i < this->_count; i++)
                    children[i]->traverse(depth + 1);
            }

            virtual std::pair<bool, _Node *> successor(unsigned pos, parent_iter_stack &parents) override {
                if (pos < this->_count) {
                    parents.emplace(this, pos);
                    return children[pos]->successor(0, parents);
                }
                return std::pair<bool, _Node *>(false, nullptr);
            }

            virtual std::pair<bool, _Node *> predecessor(int pos, parent_iter_stack &parents) override {
                if (pos >= 0) {
                    pos = std::min(this->_count - 1, pos);
                    parents.emplace(this, pos);
                    return children[pos]->predecessor(children[pos]->key_array_end(), parents);
                }
                return std::pair<bool, _Node *>(false, nullptr);
            }

            virtual uint16_t max_size() const override { return 4; }

            virtual uint16_t key_array_end() const override { return 3; }

            virtual node_type get_type() const override { return node_type::node_4_t; }

        };

        class _Node_16 : public _Node {
        public:
            std::array<byte, 16> keys{};
            std::array<_Node *, 16> children{};

            _Node_16(_Node_4 *node) : _Node(4) {
                assert(node->size() == 4);

                std::copy(node->keys.begin(), node->keys.end(), keys.begin());
                std::copy(node->children.begin(), node->children.end(), children.begin());
                delete node;
            }

            _Node_16(_Node_48 *node) : _Node(16) {
                assert(node->size() == 16);

                uint8_t pos = 0;
                for (uint16_t i = 0; i < 256; i++) {
                    if (node->child_index[i] != EMPTY_MARKER) {
                        keys[pos] = i;
                        children[pos] = node->children[node->child_index[i]];
                        pos++;
                    }
                }

                delete node;
            }

            virtual unsigned insert(const byte &key_byte, _Node *node) override {
                unsigned pos = 0;
                for (; pos < this->_count && keys[pos] < key_byte; pos++);
                if (pos != this->_count) {
                    std::move(keys.begin() + pos, keys.begin() + this->_count, keys.begin() + pos + 1);
                    std::move(children.begin() + pos, children.begin() + this->_count, children.begin() + pos + 1);
                }
                keys[pos] = key_byte;
                children[pos] = node;
                this->_count++;
                return pos;
            }

            virtual std::pair<_Node **, unsigned> find(const byte &key_byte) override {
                unsigned pos = 0;
                for (; pos < this->_count && keys[pos] < key_byte; pos++);

                if (keys[pos] == key_byte)
                    return std::pair<_Node **, unsigned>(&children[pos], pos);
                return std::pair<_Node **, unsigned>(nullptr, 0);
            }

            virtual void traverse(unsigned depth) override {
                for (uint8_t i = 0; i < this->_count; i++)
                    children[i]->traverse(depth + 1);
            }

            virtual _Node *minimum() override {
                return children[0]->minimum();
            }

            virtual _Node *minimum(parent_iter_stack &parents) override {
                parents.push(std::make_pair(this, 0));
                return children[0]->minimum(parents);
            }

            virtual _Node *maximum() override {
                return children[this->_count - 1]->maximum();
            }

            virtual _Node *maximum(parent_iter_stack &parents) override {
                auto max = this->_count - 1;
                parents.push(std::make_pair(this, max));
                return children[max]->maximum(parents);
            }

            virtual std::pair<bool, _Node *> successor(unsigned pos, parent_iter_stack &parents) override {
                if (pos < this->_count) {
                    parents.emplace(this, pos);
                    return children[pos]->successor(0, parents);
                }
                return std::pair<bool, _Node *>(false, nullptr);
            }

            virtual std::pair<bool, _Node *> predecessor(int pos, parent_iter_stack &parents) override {
                if (pos >= 0) {
                    pos = std::min(this->_count - 1, pos);
                    parents.emplace(this, pos);
                    return children[pos]->predecessor(children[pos]->key_array_end(), parents);
                }
                return std::pair<bool, _Node *>(false, nullptr);
            }

            virtual uint16_t max_size() const override { return 16; }

            virtual uint16_t key_array_end() const override { return 15; }

            virtual node_type get_type() const override { return node_type::node_16_t; }
        };


        class _Node_48 : public _Node {
        public:
            std::array<byte, 256> child_index;
            std::array<_Node *, 48> children{};

            _Node_48(_Node_16 *node) : _Node(16) {
                assert(node->size() == 16);

                std::fill(child_index.begin(), child_index.end(), EMPTY_MARKER);

                for (uint8_t i = 0; i < 16; i++) {
                    child_index[node->keys[i]] = i;
                    children[i] = node->children[i];
                }

                delete node;
            }

            _Node_48(_Node_256 *node) : _Node(48) {
                assert(node->size() == 48);

                std::fill(child_index.begin(), child_index.end(), EMPTY_MARKER);

                uint8_t pos = 0;
                for (uint16_t i = 0; i < 256; i++) {
                    if (node->children[i] != nullptr) {
                        child_index[i] = pos;
                        children[pos] = node->children[i];
                        pos++;
                    }
                }

                delete node;
            }

            virtual unsigned insert(const byte &key_byte, _Node *node) override {
                auto pos = this->_count;
                child_index[key_byte] = pos;
                children[pos] = node;
                this->_count++;
                return key_byte;
            }

            virtual std::pair<_Node **, unsigned> find(const byte &key_byte) override {
                if (child_index[key_byte] != EMPTY_MARKER)
                    return std::pair<_Node **, unsigned>(&children[child_index[key_byte]], key_byte);
                return std::pair<_Node **, unsigned>(nullptr, 0);
            }

            virtual _Node *minimum() override {
                unsigned pos = 0;
                while (child_index[pos] == EMPTY_MARKER)
                    pos++;
                return children[child_index[pos]]->minimum();
            }

            virtual _Node *minimum(parent_iter_stack &parents) override {
                unsigned pos = 0;
                while (child_index[pos] == EMPTY_MARKER)
                    pos++;
                parents.push(std::make_pair(this, pos));
                return children[child_index[pos]]->minimum(parents);
            }

            virtual _Node *maximum() override {
                auto pos = 255;
                while (child_index[pos] == EMPTY_MARKER)
                    pos--;
                return children[child_index[pos]]->maximum();
            }

            virtual _Node *maximum(parent_iter_stack &parents) override {
                auto pos = 255;
                while (child_index[pos] == EMPTY_MARKER)
                    pos--;
                parents.push(std::make_pair(this, pos));
                return children[child_index[pos]]->maximum(parents);
            }

            virtual void traverse(unsigned depth) override {
                for (uint8_t i = 0; i < this->_count; i++)
                    children[i]->traverse(depth + 1);
            }

            virtual std::pair<bool, _Node *> successor(unsigned pos, parent_iter_stack &parents) override {
                for (int i = pos; i < 256; i++) {
                    if (child_index[i] != EMPTY_MARKER) {
                        parents.emplace(this, i);
                        return children[child_index[i]]->successor(0, parents);
                    }
                }
                return std::pair<bool, _Node *>(false, nullptr);
            }

            virtual std::pair<bool, _Node *> predecessor(int pos, parent_iter_stack &parents) override {
                for (int i = pos; i >= 0; i--) {
                    if (child_index[i] != EMPTY_MARKER) {
                        parents.emplace(this, i);
                        return children[child_index[i]]->predecessor(children[child_index[i]]->key_array_end(),
                                                                     parents);
                    }
                }
                return std::pair<bool, _Node *>(false, nullptr);
            }

            virtual uint16_t max_size() const override { return 48; }

            virtual uint16_t key_array_end() const override { return 255; }

            virtual node_type get_type() const override { return node_type::node_48_t; }
        };

        class _Node_256 : public _Node {
        public:
            std::array<_Node *, 256> children{};

            _Node_256(_Node_48 *node) : _Node(48) {
                assert(node->size() == 48);

                for (uint16_t i = 0; i < 256; i++)
                    if (node->child_index[i] != EMPTY_MARKER)
                        children[i] = node->children[node->child_index[i]];

                delete node;
            }

            ~_Node_256() {
                for (int i = 0; i < 256; i++)
                    if (children[i] != nullptr)
                        delete children[i];
            }

            virtual unsigned insert(const byte &key_byte, _Node *node) override {
                children[key_byte] = node;
                this->_count++;
                return key_byte;
            }

            virtual std::pair<_Node **, unsigned> find(const byte &key_byte) override {
                if (children[key_byte] != nullptr)
                    return std::pair<_Node **, unsigned>(&children[key_byte], key_byte);
                return std::pair<_Node **, unsigned>(nullptr, 0);
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

            virtual _Node *minimum(parent_iter_stack &parents) override {
                unsigned pos = 0;
                while (children[pos] == nullptr)
                    pos++;
                parents.push(std::make_pair(this, pos));
                return children[pos]->minimum(parents);
            }

            virtual _Node *maximum() override {
                auto pos = 255;
                while (children[pos] == nullptr)
                    pos--;
                return children[pos]->maximum();
            }

            virtual _Node *maximum(parent_iter_stack &parents) override {
                auto pos = 255;
                while (children[pos] == nullptr)
                    pos--;
                parents.push(std::make_pair(this, pos));
                return children[pos]->maximum(parents);
            }

            virtual std::pair<bool, _Node *> successor(unsigned pos, parent_iter_stack &parents) override {
                for (; pos < 256; pos++) {
                    if (children[pos] != nullptr) {
                        parents.emplace(this, pos);
                        return children[pos]->successor(0, parents);
                    }
                }
                return std::pair<bool, _Node *>(false, nullptr);
            }

            virtual std::pair<bool, _Node *> predecessor(int pos, parent_iter_stack &parents) override {
                for (; pos >= 0; pos--) {
                    if (children[pos] != nullptr) {
                        parents.emplace(this, pos);
                        return children[pos]->predecessor(children[pos]->key_array_end(), parents);
                    }
                }
                return std::pair<bool, _Node *>(false, nullptr);
            }

            virtual uint16_t max_size() const override { return 256; }

            virtual uint16_t key_array_end() const override { return 255; }

            virtual node_type get_type() const override { return node_type::node_256_t; }

        };


    public:
        // Current root node of the radix tree
        _Node *_M_root;

        // Dummy node of special type to mark the right end of the container
        // Necessary as the end marker for iterators
        _Dummy_Node *_M_dummy_node;

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

        std::string print_key(Key key, unsigned depth) {
            std::bitset<8> x(key.chunks[depth]);
            std::string output = x.to_string();
            return output;
        }

        std::string print_key(Key key) {
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

            // stack of parents and the key chosen at that node
            parent_iter_stack parents;

            adapt_radix_tree_iterator() : node(nullptr) { }

            // Copy Constructor
            adapt_radix_tree_iterator(const adapt_radix_tree_iterator &__it)
                    : node(__it.node), parents(__it.parents) { }

            // @TODO INCORRECT, need to get parent stack
            explicit adapt_radix_tree_iterator(_Node *node)
                    : node(node) {
            }

            explicit adapt_radix_tree_iterator(_Node *node, parent_iter_stack &parents)
                    : node(node), parents(parents) {

            }

            // preincrement
            _Self &operator++() {
                while (!parents.empty()) {
                    auto p = parents.top();
                    parents.pop();
                    auto next = p.first->successor(p.second + 1, parents);
                    if (next.first) {
                        node = next.second;
                        return *this;
                    }
                }
                node = nullptr;
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
                while (!parents.empty()) {
                    auto p = parents.top();
                    parents.pop();
                    auto next = p.first->predecessor(p.second - 1, parents);
                    if (next.first) {
                        node = next.second;
                        return *this;
                    }
                }
                node = nullptr;
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

            // stack of parents and the key chosen at that node
            parent_iter_stack parents;

            adapt_radix_tree_const_iterator() : node(nullptr) { }

            // Copy Constructor
            adapt_radix_tree_const_iterator(const iterator &__it)
                    : node(__it.node), parents(__it.parents) { }

            // @TODO INCORRECT, need to get parent stack
            explicit adapt_radix_tree_const_iterator(_Node *node)
                    : node(node) {
            }

            explicit adapt_radix_tree_const_iterator(_Node *node, parent_iter_stack &parents)
                    : node(node), parents(parents) {

            }

            // preincrement
            _Self &operator++() {
                _Node *current_node;
                uint16_t current_pos;
                while (!parents.empty()) {
                    std::tie(current_node, current_pos) = parents.top();
                    parents.pop();
                    auto next = current_node->successor(current_pos + 1, parents);
                    if (next.first) {
                        node = next.second;
                        return *this;
                    }
                }
                node = nullptr;
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
                _Node *current_node;
                int current_pos;
                while (!parents.empty()) {
                    std::tie(current_node, current_pos) = parents.top();
                    parents.pop();
                    auto next = current_node->predecessor(current_pos - 1, parents);
                    if (next.first) {
                        node = next.second;
                        return *this;
                    }
                }
                node = nullptr;
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
            parent_iter_stack parents;
            // Place maximum dummy node on stack, since the
            // maximum leaf has no idea about this node
            // when going through the root's children has completed,
            // this will be popped from the stack as the end marker
            parents.emplace(_M_dummy_node, 0);
            _Node *min = minimum(parents);
            if (min == nullptr)
                return iterator(_M_dummy_node->_leaf, parents);

            return iterator(min, parents);
        }

        const_iterator begin() const {
            parent_iter_stack parents;
            parents.emplace(_M_dummy_node, 0);
            _Node *min = minimum(parents);
            if (min == nullptr)
                return iterator(_M_dummy_node->_leaf, parents);
            return const_iterator(min, parents);
        }

        iterator end() {
            parent_iter_stack parents;
            parents.emplace(_M_dummy_node, 0);
            return iterator(_M_dummy_node->_leaf, parents);
        }

        const_iterator end() const {
            parent_iter_stack parents;
            parents.emplace(_M_dummy_node, 0);
            return const_iterator(_M_dummy_node->_leaf, parents);
        }

        reverse_iterator rbegin() {
            return reverse_iterator(end());
        }

        const_reverse_iterator rbegin() const {
            return const_reverse_iterator(end());
        }

        reverse_iterator rend() {
            parent_iter_stack parents;
            _Node *min = minimum(parents);
            if (min == nullptr)
                return reverse_iterator(iterator(_M_dummy_node->_leaf, parents));
            return reverse_iterator(iterator(min, parents));
        }

        const_reverse_iterator rend() const {
            parent_iter_stack parents;
            _Node *min = minimum(parents);
            if (min == nullptr)
                return const_reverse_iterator(iterator(_M_dummy_node->_leaf, parents));
            return const_reverse_iterator(iterator(min, parents));
        }

        ///////////////
        // Modifiers //
        ///////////////

        // @TODO
        void clear() {
            delete this->_M_root;
        }

        // @TODO memory management?
        void _M_reset() {
            this->_M_root = nullptr;
            this->_M_count = 0;
        }

        std::pair<iterator, bool> _M_insert_unique(const value_type &__x) {
            Key transformed_key = {_M_key_transform(__x.first)};
            const auto key_size = sizeof(__x.first);

            _Leaf *new_leaf = new _Leaf(transformed_key, __x);

            // Empty Tree
            if (_M_root == nullptr) {
                _M_root = new_leaf;
                _M_count++;

                _M_dummy_node->_root = &_M_root;
                parent_iter_stack parents;
                parents.push(std::pair<_Node *, unsigned>(_M_dummy_node, 0));
                return std::make_pair(iterator(_M_root, parents), true);
            }

            _Node **current_node = &_M_root;
            _Node **previous_node = nullptr;
            parent_iter_stack parents;
            parents.push(std::pair<_Node *, unsigned>(_M_dummy_node, 0));

            for (unsigned i = 0; i < key_size + 1; i++) {
                if (current_node != nullptr && *current_node != nullptr && (*current_node)->is_leaf()) {
                    // Hit an existing leaf
                    _Leaf *existing_leaf = reinterpret_cast<_Leaf *>(*current_node);
                    if (transformed_key.value == existing_leaf->key.value) {
                        // if it is a duplicate entry, ignore
                        delete new_leaf;
                        return std::make_pair(iterator(existing_leaf, parents), false);
                    } else {
                        // otherwise, the leaf needs to be replaced by a node 4
                        Key existing_key = existing_leaf->key;
                        *current_node = new _Node_4(existing_leaf, i);
                        // if the keys are matching, go down all the way until we find a tiebreaker
                        // insert node4's with one child all the way down until a final node 4 with 2 children
                        for (unsigned j = i; j < key_size + 1; j++) {
                            if (existing_key.chunks[j] == transformed_key.chunks[j]) {
                                _Node **old_child;
                                unsigned pos;
                                std::tie(old_child, pos) = (*current_node)->find(existing_key.chunks[j]);
                                _Node *new_child = new _Node_4(existing_leaf, j + 1);
                                *old_child = new_child;
                                current_node = old_child;
                                parents.push(std::pair<_Node *, unsigned>(*current_node, 0));
                            } else {
                                if ((*current_node)->size() == (*current_node)->max_size())
                                    *current_node = grow(*current_node);
                                unsigned pos = (*current_node)->insert(transformed_key.chunks[j], new_leaf);
                                parents.push(std::pair<_Node *, unsigned>(*current_node, pos));
                                _M_count++;
                                return std::make_pair(iterator(new_leaf, parents), true);
                            }
                        }
                        throw; // unreachable
                    }
                } else if (current_node != nullptr && *current_node != nullptr) {
                    // traverse down the tree
                    unsigned pos;
                    previous_node = current_node;
                    std::tie(current_node, pos) = (*current_node)->find(transformed_key.chunks[i]);
                    parents.emplace(*previous_node, pos);
                } else {
                    // hit empty point, this can only happen if the inserted key
                    // is not a prefix/equal to another key already in the tree
                    // therefore we can just insert a new leaf
                    // previous node might have to be grown before that
                    if ((*previous_node)->size() == (*previous_node)->max_size()) {
                        parents.pop();
                        *previous_node = grow(*previous_node);
                    }
                    unsigned pos = (*previous_node)->insert(transformed_key.chunks[i - 1], new_leaf);
                    parents.push(std::pair<_Node *, unsigned>(*previous_node, pos));
                    _M_count++;
                    return std::make_pair(iterator(new_leaf, parents), true);
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
            parent_iter_stack parents;
            Key transformed_key = {_M_key_transform(__k)};
            const auto key_size = sizeof(__k);

            if (_M_root == nullptr)
                return end();

            _Node **previous_node = nullptr;
            _Node **current_node = &_M_root;
            parents.emplace(_M_dummy_node, 0);
            unsigned pos = 0;
            for (unsigned i = 0; i < key_size + 1; i++) {
                if (current_node == nullptr || *current_node == nullptr)
                    return end();

                if ((*current_node)->is_leaf()) {
                    if (transformed_key.value == ((_Leaf *) *current_node)->key.value) {
                        return iterator(*current_node, parents);
                    } else {
                        return end();
                    }
                }

                previous_node = current_node;
                std::tie(current_node, pos) = (*current_node)->find(transformed_key.chunks[i]);
                parents.emplace(*previous_node, pos);
            }
            throw; // unreachable
        }

        // @TODO test this function
        iterator lower_bound(const key_type &__k) {
            parent_iter_stack parents;
            Key transformed_key = {_M_key_transform(__k)};
            const auto key_size = sizeof(__k);

            if (_M_root == nullptr)
                return end();

            _Node **previous_node = nullptr;
            _Node **current_node = &_M_root;
            parents.emplace(_M_dummy_node, 0);
            unsigned pos = 0;
            for (unsigned i = 0; i < key_size + 1; i++) {
                if (current_node == nullptr || *current_node == nullptr) {
                    auto prev = parents.top();
                    parents.pop();
                    auto successor = prev.first->successor(prev.second + 1, parents);
                    return iterator(successor.second, parents);
                }

                if ((*current_node)->is_leaf()) {
                    if (((_Leaf *) *current_node)->key.value <= transformed_key.value) {
                        return iterator(*current_node, parents);
                    } else {
                        auto prev = parents.top();
                        parents.pop();
                        auto successor = prev.first->successor(prev.second + 1, parents);
                        return iterator(successor.second, parents);
                    }
                }

                previous_node = current_node;
                std::tie(current_node, pos) = (*current_node)->find(transformed_key.chunks[i]);
                parents.emplace(*previous_node, pos);
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

        _Node *minimum(parent_iter_stack &parents) const {
            if (_M_root != nullptr)
                return _M_root->minimum(parents);
            return nullptr;
        }

        _Node *maximum() const {
            if (_M_root != nullptr)
                return _M_root->maximum();
            return nullptr;
        }

        _Node *maximum(parent_iter_stack &parents) const {
            if (_M_root != nullptr)
                return _M_root->maximum(parents);
            return nullptr;
        }

        void traverse() const {
            if (_M_root != nullptr)
                _M_root->traverse(0);
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
