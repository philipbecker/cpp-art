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

    template<typename _Key, typename _V,
            typename _Key_transform = key_transform<_Key>,
            typename _Alloc = std::allocator<std::pair<const _Key, _V> > >
    class Adaptive_radix_tree {
    public:
        // Forward declaration for typedefs
        class _Node;

        class _Node_16;

        class _Node_48;

        class _Node_256;

        typedef _Key key_type;
        typedef _V mapped_type;
        typedef std::pair<const _Key, _V> value_type;

    private:
        typedef _Node *Node_ptr;
        typedef const _Node *Const_Node_ptr;

        enum node_type : uint8_t {
            _leaf_t = 0, node_4_t = 1, node_16_t = 2,
            node_48_t = 3, node_256_t = 4, _dummy_node_t = 5
        };

        _Key_transform _M_key_transform;

        typedef decltype(_M_key_transform(key_type())) transformed_key_type;

    public:
        union Key {
            const transformed_key_type value;
            const byte chunks[sizeof(transformed_key_type)];
        };

        class _Node {
        public:
            uint16_t _count;

            Node_ptr _parent;

            _Node(uint16_t count, Node_ptr parent)
                    : _count(count), _parent(parent) { }


            // Copy constructor
            _Node(const _Node &__x)
                    : _count(__x._count), _parent(__x._parent) {
            }

            // Move constructor
            _Node(_Node &&__x)
                    : _count(std::move(__x._count)), _parent(std::move(__x._parent)) { }

            // Copy assignment
            _Node &operator=(const _Node &__x) {
                _count = __x._count;
                _parent = __x._parent;
                return *this;
            }

            // Move assignment
            _Node &operator=(_Node &&__x) {
                _count = std::move(__x._count);
                _parent = std::move(__x._parent);
                return *this;
            }

            virtual ~_Node() { }

            virtual void clear() = 0;

            virtual void insert(const byte &key_byte, Node_ptr node) = 0;

            virtual void erase(const byte &key_byte) = 0;

            virtual Node_ptr *find(const byte &key_byte) = 0;

            virtual Const_Node_ptr find(const byte &key_byte) const = 0;

            virtual Node_ptr minimum() = 0;

            virtual Const_Node_ptr minimum() const = 0;

            virtual Node_ptr maximum() = 0;

            virtual Const_Node_ptr maximum() const = 0;

            virtual bool is_leaf() const { return false; }

            uint16_t size() const { return _count; }

            virtual uint16_t min_size() const = 0;

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
            virtual Node_ptr successor(const Key &key, int depth) = 0;

            virtual Const_Node_ptr successor(const Key &key, int depth) const = 0;

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
            virtual Node_ptr predecessor(const Key &key, int depth) = 0;

            virtual Const_Node_ptr predecessor(const Key &key, int depth) const = 0;

            virtual void debug() const = 0;
        };

        class _Leaf : public _Node {
        public:
            Key key;
            value_type value;
            int depth;

            _Leaf(Key key, int depth, Node_ptr parent)
                    : _Node(1, parent), key(key), depth(depth) {
            }

            _Leaf(Key key, value_type value, int depth, Node_ptr parent)
                    : _Node(1, parent), key(key), value(value), depth(depth) {
            }

            // Copy constructor
            _Leaf(const _Leaf &__x)
                    : _Node(__x._count, nullptr), key(__x.key),
                      value(__x.value), depth(__x.depth) {
            }

            // Move constructor
            _Leaf(_Leaf &&__x)
                    : _Node(std::move(__x)), key(std::move(__x.key)),
                      value(std::move(__x.value)), depth(std::move(__x.depth)) {
            }

            // Copy assignment
            _Leaf &operator=(const _Leaf &__x) {
                this->_count = __x._count;
                this->_parent = nullptr;
                //key = __x.key;
                //value(__x.value);
                depth = __x.depth;
                return *this;
            }

            // Move assignment
            _Leaf &operator=(_Leaf &&__x) {
                this->_count = std::move(__x._count);
                this->_parent = std::move(__x._parent);
                key = std::move(__x.key);
                value = std::move(__x.value);
                depth = std::move(__x.depth);
                return *this;
            }

            void clear() { }

            virtual void insert(const byte &key_byte, Node_ptr node) override { }

            virtual void erase(const byte &key_byte) override { }

            virtual Node_ptr *find(const byte &key_byte) override {
                return nullptr;
            }

            virtual Const_Node_ptr find(const byte &key_byte) const override {
                return nullptr;
            }

            virtual Node_ptr minimum() override { return this; }

            virtual Const_Node_ptr minimum() const override { return this; }

            virtual Node_ptr maximum() override { return this; }

            virtual Const_Node_ptr maximum() const override { return this; }

            virtual Node_ptr successor(const Key &key, int depth) override {
                return this;
            }

            virtual Const_Node_ptr successor(const Key &key, int depth) const override {
                return this;
            }

            virtual Node_ptr predecessor(const Key &key, int depth) override {
                return this;
            }

            virtual Const_Node_ptr predecessor(const Key &key, int depth) const override {
                return this;
            }

            virtual uint16_t min_size() const override { return 1; }

            virtual uint16_t max_size() const override { return 1; }

            bool is_leaf() const override { return true; }

            transformed_key_type transformed_key() const {
                return transformed_key_type()(value.first);
            }

            virtual node_type get_type() const override { return node_type::_leaf_t; }

            virtual void debug() const override {
                std::cout << "Leaf: " << value.first << " -> " << value.second << std::endl;
            }
        };

        /**
         * End of iteration marker right of the container.
         */
        class _Dummy_Node : public _Node {
        public:
            Node_ptr *_root;

            _Leaf *_leaf;

            _Dummy_Node()
                    : _Node(0, nullptr), _root(nullptr),
                      _leaf(new _Leaf(Key{0}, std::pair<key_type, mapped_type>(key_type(), mapped_type()), 0, this)) {
            }

            _Dummy_Node(Node_ptr *root, _Leaf *leaf)
                    : _Node(0, nullptr), _root(root),
                      _leaf(new _Leaf(leaf->key, leaf->value, 0, this)) {
            }

            // Move constructor
            _Dummy_Node(_Dummy_Node &&__x) : _Node(std::move(__x)) {
                _root = std::move(__x._root);
                _leaf = std::move(__x._leaf);
            }

            // Copy constructor
            _Dummy_Node(const _Dummy_Node &__x) : _Node(__x._count, __x._parent) {
                *_leaf = *(__x._leaf);
            }

            // Copy assignment
            _Dummy_Node &operator=(const _Dummy_Node &__x) {
                this->_count = __x._count;
                this->_parent = __x._parent;
                *_leaf = *__x._leaf;
                return *this;
            }

            // Move assignment
            _Dummy_Node &operator=(_Dummy_Node &&__x) {
                this->_count = std::move(__x._count);
                this->_parent = std::move(__x._parent);
                _root = std::move(__x._root);
                _leaf = std::move(__x._leaf);
                return *this;
            }

            void clear() { }

            virtual void insert(const byte &key_byte, Node_ptr node) override { }

            virtual void erase(const byte &key_byte) override { }

            virtual Node_ptr *find(const byte &key_byte) override {
                return nullptr;
            }

            virtual Const_Node_ptr find(const byte &key_byte) const override {
                return nullptr;
            }

            virtual Node_ptr minimum() override { return this; }

            virtual Const_Node_ptr minimum() const override { return this; }

            virtual Node_ptr maximum() override { return this; }

            virtual Const_Node_ptr maximum() const override { return this; }

            virtual Node_ptr successor(const Key &key, int depth) override {
                return this->_leaf;
            }

            virtual Const_Node_ptr successor(const Key &key, int depth) const override {
                return this->_leaf;
            }

            virtual Node_ptr predecessor(const Key &key, int depth) override {
                if (_root != nullptr)
                    return (*_root)->maximum();

                return this->_leaf;
            }

            virtual Const_Node_ptr predecessor(const Key &key, int depth) const override {
                if (_root != nullptr)
                    return (*_root)->maximum();

                return this->_leaf;
            }

            virtual uint16_t min_size() const override { return 0; }

            virtual uint16_t max_size() const override { return 0; }

            virtual node_type get_type() const override { return node_type::_dummy_node_t; }

            virtual void debug() const override {
                std::cout << "Dummy Node debug" << std::endl;
            }
        };

        class _Node_4 : public _Node {
        public:
            std::array<byte, 4> keys{};
            std::array<Node_ptr, 4> children{};

            // Grow constructor
            _Node_4(_Leaf *leaf, int depth)
                    : _Node(1, leaf->_parent) {
                Key key = leaf->key;
                leaf->depth = depth + 1;
                keys[0] = key.chunks[depth];
                children[0] = leaf;

                children[0]->_parent = this;
            }

            // Shrink constructor
            _Node_4(_Node_16 *node) : _Node(4, node->_parent) {
                std::copy(node->keys.begin(), node->keys.begin() + 4, keys.begin());
                std::copy(node->children.begin(), node->children.begin() + 4, children.begin());

                for (size_t i = 0; i < 4; i++)
                    children[i]->_parent = this;
            }

            // Copy constructor
            _Node_4(const _Node_4 &__x) : _Node(__x._count, __x._parent), keys(__x.keys) {
                for (int i = 0; i < __x._count; i++) {
                    switch (__x.children[i]->get_type()) {
                        case node_type::node_4_t:
                            children[i] = new _Node_4(*static_cast<_Node_4 *>(__x.children[i]));
                            children[i]->_parent = this;
                            break;
                        case node_type::node_16_t:
                            children[i] = new _Node_16(*static_cast<_Node_16 *>(__x.children[i]));
                            children[i]->_parent = this;
                            break;
                        case node_type::node_48_t:
                            children[i] = new _Node_48(*static_cast<_Node_48 *>(__x.children[i]));
                            children[i]->_parent = this;
                            break;
                        case node_type::node_256_t:
                            children[i] = new _Node_256(*static_cast<_Node_256 *>(__x.children[i]));
                            children[i]->_parent = this;
                            break;
                        case node_type::_leaf_t:
                            children[i] = new _Leaf(*static_cast<_Leaf *>(__x.children[i]));
                            children[i]->_parent = this;
                            break;
                        default:
                            throw;
                    }
                }
            }

            // Move constructor
            _Node_4(_Node_4 &&__x) : _Node(std::move(__x)) {
                keys = std::move(__x.keys);
                children = std::move(__x.children);
            }

            // Copy assignment
            _Node_4 &operator=(const _Node_4 &__x) {
                this->_count = __x._count;
                this->_parent = __x._parent;
                keys = __x.keys;
                children = __x.children;
                return *this;
            }

            // Move assignment
            _Node_4 &operator=(_Node_4 &&__x) {
                this->_count = std::move(__x._count);
                this->_parent = std::move(__x._parent);
                keys = std::move(__x.keys);
                children = std::move(__x.children);
                return *this;
            }

            void clear() {
                for (uint16_t i = 0; i < this->_count; i++) {
                    children[i]->clear();
                    delete children[i];
                }
            }

            virtual void insert(const byte &key_byte, Node_ptr node) override {
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

            virtual void erase(const byte &key_byte) override {
                unsigned pos = 0;
                for (; pos < this->_count && keys[pos] < key_byte; pos++);
                if (pos < this->_count) {
                    delete children[pos];
                    std::move(keys.begin() + pos + 1, keys.begin() + this->_count, keys.begin() + pos);
                    std::move(children.begin() + pos + 1, children.begin() + this->_count, children.begin() + pos);
                    this->_count--;
                }
            }

            virtual Node_ptr *find(const byte &key_byte) override {
                unsigned pos = 0;
                for (; pos < this->_count && keys[pos] < key_byte; pos++);

                if (pos < this->_count && keys[pos] == key_byte)
                    return &children[pos];
                return nullptr;
            }

            virtual Const_Node_ptr find(const byte &key_byte) const override {
                unsigned pos = 0;
                for (; pos < this->_count && keys[pos] < key_byte; pos++);

                if (pos < this->_count && keys[pos] == key_byte)
                    return children[pos];
                return nullptr;
            }

            virtual Node_ptr minimum() override {
                return children[0]->minimum();
            }

            virtual Const_Node_ptr minimum() const override {
                return children[0]->minimum();
            }

            virtual Node_ptr maximum() override {
                return children[this->_count - 1]->maximum();
            }

            virtual Const_Node_ptr maximum() const override {
                return children[this->_count - 1]->maximum();
            }

            virtual Node_ptr successor(const Key &key, int depth) override {
                unsigned pos = 0;
                for (; pos < this->_count && keys[pos] <= key.chunks[depth]; pos++);
                if (pos < this->_count)
                    return children[pos]->minimum();
                else
                    return this->_parent->successor(key, depth - 1);
            }

            virtual Const_Node_ptr successor(const Key &key, int depth) const override {
                unsigned pos = 0;
                for (; pos < this->_count && keys[pos] <= key.chunks[depth]; pos++);
                if (pos < this->_count)
                    return children[pos]->minimum();
                else
                    return this->_parent->successor(key, depth - 1);
            }

            virtual Node_ptr predecessor(const Key &key, int depth) override {
                int pos = this->_count - 1;
                for (; pos >= 0 && keys[pos] >= key.chunks[depth]; pos--);
                if (pos >= 0)
                    return children[pos]->maximum();
                else
                    return this->_parent->predecessor(key, depth - 1);
            }

            virtual Const_Node_ptr predecessor(const Key &key, int depth) const override {
                int pos = this->_count - 1;
                for (; pos >= 0 && keys[pos] >= key.chunks[depth]; pos--);
                if (pos >= 0)
                    return children[pos]->maximum();
                else
                    return this->_parent->predecessor(key, depth - 1);
            }

            virtual uint16_t min_size() const override { return 2; }

            virtual uint16_t max_size() const override { return 4; }

            virtual node_type get_type() const override { return node_type::node_4_t; }

            virtual void debug() const override {
                std::cout << "Node 4, count: " << size() << std::endl;
                if (size() > 0) {
                    for (size_t i = 0; i < this->_count; i++) {
                        std::cout << ((unsigned) keys[i]) << " | ";
                    }
                    std::cout << std::endl;
                    for (size_t i = 0; i < this->_count; i++) {
                        std::cout << children[i] << " | ";
                    }
                    std::cout << std::endl;
                }
            }
        };

        class _Node_16 : public _Node {
        public:
            std::array<byte, 16> keys{};
            std::array<Node_ptr, 16> children{};

            // Grow constructor
            _Node_16(_Node_4 *node) : _Node(4, node->_parent) {
                assert(node->size() == 4);

                std::copy(node->keys.begin(), node->keys.end(), keys.begin());
                std::copy(node->children.begin(), node->children.end(), children.begin());

                for (size_t i = 0; i < 4; i++)
                    children[i]->_parent = this;
            }

            // Shrink constructor
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
            }

            // Copy constructor
            _Node_16(const _Node_16 &__x) : _Node(__x._count, __x._parent), keys(__x.keys) {
                for (int i = 0; i < __x._count; i++) {
                    switch (__x.children[i]->get_type()) {
                        case node_type::node_4_t:
                            children[i] = new _Node_4(*static_cast<_Node_4 *>(__x.children[i]));
                            children[i]->_parent = this;
                            break;
                        case node_type::node_16_t:
                            children[i] = new _Node_16(*static_cast<_Node_16 *>(__x.children[i]));
                            children[i]->_parent = this;
                            break;
                        case node_type::node_48_t:
                            children[i] = new _Node_48(*static_cast<_Node_48 *>(__x.children[i]));
                            children[i]->_parent = this;
                            break;
                        case node_type::node_256_t:
                            children[i] = new _Node_256(*static_cast<_Node_256 *>(__x.children[i]));
                            children[i]->_parent = this;
                            break;
                        case node_type::_leaf_t:
                            children[i] = new _Leaf(*static_cast<_Leaf *>(__x.children[i]));
                            children[i]->_parent = this;
                            break;
                        default:
                            throw;
                            break;
                    }
                }
            }

            // Move constructor
            _Node_16(_Node_16 &&__x) : _Node(std::move(__x)) {
                keys = std::move(__x.keys);
                children = std::move(__x.children);
            }

            // Copy assignment
            _Node_16 &operator=(const _Node_16 &__x) {
                this->_count = __x._count;
                this->_parent = __x._parent;
                keys = __x.keys;
                children = __x.children;
                return *this;
            }

            // Move assignment
            _Node_16 &operator=(_Node_16 &&__x) {
                this->_count = std::move(__x._count);
                this->_parent = std::move(__x._parent);
                keys = std::move(__x.keys);
                children = std::move(__x.children);
                return *this;
            }

            void clear() {
                for (uint16_t i = 0; i < this->_count; i++) {
                    children[i]->clear();
                    delete children[i];
                }
            }

            virtual void insert(const byte &key_byte, Node_ptr node) override {
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

            virtual void erase(const byte &key_byte) override {
                unsigned pos = 0;
                for (; pos < this->_count && keys[pos] < key_byte; pos++);
                if (pos < this->_count) {
                    delete children[pos];
                    std::move(keys.begin() + pos + 1, keys.begin() + this->_count, keys.begin() + pos);
                    std::move(children.begin() + pos + 1, children.begin() + this->_count, children.begin() + pos);
                    this->_count--;
                }
            }

            virtual Node_ptr *find(const byte &key_byte) override {
                unsigned pos = 0;
                for (; pos < this->_count && keys[pos] < key_byte; pos++);

                if (pos < this->_count && keys[pos] == key_byte)
                    return &children[pos];
                return nullptr;
            }

            virtual Const_Node_ptr find(const byte &key_byte) const override {
                unsigned pos = 0;
                for (; pos < this->_count && keys[pos] < key_byte; pos++);

                if (pos < this->_count && keys[pos] == key_byte)
                    return children[pos];
                return nullptr;
            }

            virtual Node_ptr minimum() override {
                return children[0]->minimum();
            }

            virtual Const_Node_ptr minimum() const override {
                return children[0]->minimum();
            }

            virtual Node_ptr maximum() override {
                return children[this->_count - 1]->maximum();
            }

            virtual Const_Node_ptr maximum() const override {
                return children[this->_count - 1]->maximum();
            }

            virtual Node_ptr successor(const Key &key, int depth) override {
                unsigned pos = 0;
                for (; pos < this->_count && keys[pos] <= key.chunks[depth]; pos++);
                if (pos < this->_count)
                    return children[pos]->minimum();
                else
                    return this->_parent->successor(key, depth - 1);
            }

            virtual Const_Node_ptr successor(const Key &key, int depth) const override {
                unsigned pos = 0;
                for (; pos < this->_count && keys[pos] <= key.chunks[depth]; pos++);
                if (pos < this->_count)
                    return children[pos]->minimum();
                else
                    return this->_parent->successor(key, depth - 1);
            }

            virtual Node_ptr predecessor(const Key &key, int depth) override {
                int pos = this->_count - 1;
                for (; pos >= 0 && keys[pos] >= key.chunks[depth]; pos--);
                if (pos >= 0)
                    return children[pos]->maximum();
                else
                    return this->_parent->predecessor(key, depth - 1);
            }

            virtual Const_Node_ptr predecessor(const Key &key, int depth) const override {
                int pos = this->_count - 1;
                for (; pos >= 0 && keys[pos] >= key.chunks[depth]; pos--);
                if (pos >= 0)
                    return children[pos]->maximum();
                else
                    return this->_parent->predecessor(key, depth - 1);
            }

            virtual uint16_t min_size() const override { return 5; }

            virtual uint16_t max_size() const override { return 16; }

            virtual node_type get_type() const override { return node_type::node_16_t; }

            virtual void debug() const override {
                std::cout << "Node 16, count: " << size() << std::endl;
                for (size_t i = 0; i < this->_count; i++) {
                    std::cout << ((unsigned) keys[i]) << " | ";
                }
                std::cout << std::endl;
                for (size_t i = 0; i < this->_count; i++) {
                    std::cout << children[i] << " | ";
                }
                std::cout << std::endl;
            }
        };


        class _Node_48 : public _Node {
        public:
            std::array<byte, 256> child_index;
            std::array<Node_ptr, 48> children{};

            // Grow constructor
            _Node_48(_Node_16 *node) : _Node(16, node->_parent) {
                assert(node->size() == 16);

                std::fill(child_index.begin(), child_index.end(), EMPTY_MARKER);

                for (uint8_t i = 0; i < 16; i++) {
                    child_index[node->keys[i]] = i;
                    children[i] = node->children[i];
                    children[i]->_parent = this;
                }
            }

            // Shrink constructor
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
            }

            // Copy constructor
            _Node_48(const _Node_48 &__x) : _Node(__x._count, __x._parent), child_index(__x.child_index) {
                for (int i = 0; i < 256; i++) {
                    if (__x.child_index[i] != EMPTY_MARKER) {
                        switch (__x.children[child_index[i]]->get_type()) {
                            case node_type::node_4_t:
                                children[child_index[i]] = new _Node_4(*static_cast<_Node_4 *>(__x.children[i]));
                                children[child_index[i]]->_parent = this;
                                break;
                            case node_type::node_16_t:
                                children[child_index[i]] = new _Node_16(*static_cast<_Node_16 *>(__x.children[i]));
                                children[child_index[i]]->_parent = this;
                                break;
                            case node_type::node_48_t:
                                children[child_index[i]] = new _Node_48(*static_cast<_Node_48 *>(__x.children[i]));
                                children[child_index[i]]->_parent = this;
                                break;
                            case node_type::node_256_t:
                                children[child_index[i]] = new _Node_256(*static_cast<_Node_256 *>(__x.children[i]));
                                children[child_index[i]]->_parent = this;
                                break;
                            case node_type::_leaf_t:
                                children[child_index[i]] = new _Leaf(*static_cast<_Leaf *>(__x.children[i]));
                                children[child_index[i]]->_parent = this;
                                break;
                            default:
                                throw;
                                break;
                        }
                    }
                }
            }

            // Move constructor
            _Node_48(_Node_48 &&__x) : _Node(std::move(__x)) {
                child_index = std::move(__x.child_index);
                children = std::move(__x.children);
            }


            // Copy assignment
            _Node_48 &operator=(const _Node_48 &__x) {
                this->_count = __x._count;
                this->_parent = __x._parent;
                child_index = __x.child_index;
                children = __x.children;
                return *this;
            }

            // Move assignment
            _Node_48 &operator=(_Node_48 &&__x) {
                this->_count = std::move(__x._count);
                this->_parent = std::move(__x._parent);
                child_index = std::move(__x.child_index);
                children = std::move(__x.children);
                return *this;
            }

            void clear() {
                for (size_t i = 0; i < 256; i++) {
                    if (child_index[i] != EMPTY_MARKER) {
                        children[child_index[i]]->clear();
                        delete children[child_index[i]];
                    }
                }
            }

            virtual void insert(const byte &key_byte, Node_ptr node) override {
                unsigned pos = 0;
                for (; children[pos] != nullptr; pos++);
                child_index[key_byte] = pos;
                children[pos] = node;
                this->_count++;
            }

            virtual void erase(const byte &key_byte) override {
                delete children[child_index[key_byte]];
                children[child_index[key_byte]] = nullptr;
                child_index[key_byte] = EMPTY_MARKER;
                this->_count--;
            }

            virtual Node_ptr *find(const byte &key_byte) override {
                if (child_index[key_byte] != EMPTY_MARKER)
                    return &children[child_index[key_byte]];
                return nullptr;
            }

            virtual Const_Node_ptr find(const byte &key_byte) const override {
                if (child_index[key_byte] != EMPTY_MARKER)
                    return children[child_index[key_byte]];
                return nullptr;
            }

            virtual Node_ptr minimum() override {
                unsigned pos = 0;
                while (child_index[pos] == EMPTY_MARKER)
                    pos++;
                return children[child_index[pos]]->minimum();
            }

            virtual Const_Node_ptr minimum() const override {
                unsigned pos = 0;
                while (child_index[pos] == EMPTY_MARKER)
                    pos++;
                return children[child_index[pos]]->minimum();
            }

            virtual Node_ptr maximum() override {
                auto pos = 255;
                while (child_index[pos] == EMPTY_MARKER)
                    pos--;
                return children[child_index[pos]]->maximum();
            }

            virtual Const_Node_ptr maximum() const override {
                auto pos = 255;
                while (child_index[pos] == EMPTY_MARKER)
                    pos--;
                return children[child_index[pos]]->maximum();
            }

            virtual Node_ptr successor(const Key &key, int depth) override {
                for (unsigned pos = ((unsigned) key.chunks[depth]) + 1; pos < 256; pos++)
                    if (child_index[pos] != EMPTY_MARKER)
                        return children[child_index[pos]]->minimum();

                return this->_parent->successor(key, depth - 1);
            }

            virtual Const_Node_ptr successor(const Key &key, int depth) const override {
                for (unsigned pos = ((unsigned) key.chunks[depth]) + 1; pos < 256; pos++)
                    if (child_index[pos] != EMPTY_MARKER)
                        return children[child_index[pos]]->minimum();

                return this->_parent->successor(key, depth - 1);
            }

            virtual Node_ptr predecessor(const Key &key, int depth) override {
                for (int pos = ((int) key.chunks[depth]) - 1; pos >= 0; pos--)
                    if (child_index[pos] != EMPTY_MARKER)
                        return children[child_index[pos]]->maximum();

                return this->_parent->predecessor(key, depth - 1);
            }

            virtual Const_Node_ptr predecessor(const Key &key, int depth) const override {
                for (int pos = ((int) key.chunks[depth]) - 1; pos >= 0; pos--)
                    if (child_index[pos] != EMPTY_MARKER)
                        return children[child_index[pos]]->maximum();

                return this->_parent->predecessor(key, depth - 1);
            }

            virtual uint16_t min_size() const override { return 17; }

            virtual uint16_t max_size() const override { return 48; }

            virtual node_type get_type() const override { return node_type::node_48_t; }

            virtual void debug() const override {
                std::cout << "Node 48, count: " << size() << std::endl;
                for (size_t i = 0; i < 256; i++) {
                    if (child_index[i] != EMPTY_MARKER)
                        std::cout << i << " | ";
                }
                std::cout << std::endl;
                for (size_t i = 0; i < this->_count; i++) {
                    std::cout << i << ": " << children[i] << " | ";
                }
                std::cout << std::endl;
            }
        };

        class _Node_256 : public _Node {
        public:
            std::array<Node_ptr, 256> children{};

            // Grow constructor
            _Node_256(_Node_48 *node) : _Node(48, node->_parent) {
                assert(node->size() == 48);

                for (uint16_t i = 0; i < 256; i++) {
                    if (node->child_index[i] != EMPTY_MARKER) {
                        children[i] = node->children[node->child_index[i]];
                        children[i]->_parent = this;
                    }
                }
            }

            // Copy constructor
            _Node_256(const _Node_256 &__x) : _Node(__x._count, __x._parent) {
                for (int i = 0; i < 256; i++) {
                    if (__x.children[i] != nullptr) {
                        switch (__x.children[i]->get_type()) {
                            case node_type::node_4_t:
                                children[i] = new _Node_4(*static_cast<_Node_4 *>(__x.children[i]));
                                children[i]->_parent = this;
                                break;
                            case node_type::node_16_t:
                                children[i] = new _Node_16(*static_cast<_Node_16 *>(__x.children[i]));
                                children[i]->_parent = this;
                                break;
                            case node_type::node_48_t:
                                children[i] = new _Node_48(*static_cast<_Node_48 *>(__x.children[i]));
                                children[i]->_parent = this;
                                break;
                            case node_type::node_256_t:
                                children[i] = new _Node_256(*static_cast<_Node_256 *>(__x.children[i]));
                                children[i]->_parent = this;
                                break;
                            case node_type::_leaf_t:
                                children[i] = new _Leaf(*static_cast<_Leaf *>(__x.children[i]));
                                children[i]->_parent = this;
                                break;
                            default:
                                throw;
                                break;
                        }
                    }
                }
            }

            // Move constructor
            _Node_256(_Node_256 &&__x) : _Node(std::move(__x)) {
                children = std::move(__x.children);
            }

            // Copy assignment
            _Node_256 &operator=(const _Node_256 &__x) {
                this->_count = __x._count;
                this->_parent = __x._parent;
                return *this;
            }

            // Move assignment
            _Node_256 &operator=(_Node_256 &&__x) {
                this->_count = std::move(__x._count);
                this->_parent = std::move(__x._parent);
                children = std::move(__x.children);
                return *this;
            }

            void clear() {
                for (size_t i = 0; i < 256; i++) {
                    if (children[i] != nullptr) {
                        children[i]->clear();
                        delete children[i];
                    }
                }
            }

            virtual void insert(const byte &key_byte, Node_ptr node) override {
                children[key_byte] = node;
                this->_count++;
            }

            virtual void erase(const byte &key_byte) override {
                delete children[key_byte];
                children[key_byte] = nullptr;
                this->_count--;
            }

            virtual Node_ptr *find(const byte &key_byte) override {
                if (children[key_byte] != nullptr)
                    return &children[key_byte];
                return nullptr;
            }

            virtual Const_Node_ptr find(const byte &key_byte) const override {
                if (children[key_byte] != nullptr)
                    return children[key_byte];
                return nullptr;
            }

            virtual Node_ptr minimum() override {
                unsigned pos = 0;
                while (children[pos] == nullptr)
                    pos++;
                return children[pos]->minimum();
            }

            virtual Const_Node_ptr minimum() const override {
                unsigned pos = 0;
                while (children[pos] == nullptr)
                    pos++;
                return children[pos]->minimum();
            }

            virtual Node_ptr maximum() override {
                auto pos = 255;
                while (children[pos] == nullptr)
                    pos--;
                return children[pos]->maximum();
            }

            virtual Const_Node_ptr maximum() const override {
                auto pos = 255;
                while (children[pos] == nullptr)
                    pos--;
                return children[pos]->maximum();
            }

            virtual Node_ptr successor(const Key &key, int depth) override {
                for (unsigned pos = ((unsigned) key.chunks[depth]) + 1; pos < 256; pos++)
                    if (children[pos] != nullptr)
                        return children[pos]->minimum();

                return this->_parent->successor(key, depth - 1);
            }

            virtual Const_Node_ptr successor(const Key &key, int depth) const override {
                for (unsigned pos = ((unsigned) key.chunks[depth]) + 1; pos < 256; pos++)
                    if (children[pos] != nullptr)
                        return children[pos]->minimum();

                return this->_parent->successor(key, depth - 1);
            }

            virtual Node_ptr predecessor(const Key &key, int depth) override {
                for (int pos = key.chunks[depth] - 1; pos >= 0; pos--)
                    if (children[pos] != nullptr)
                        return children[pos]->maximum();

                return this->_parent->predecessor(key, depth - 1);
            }

            virtual Const_Node_ptr predecessor(const Key &key, int depth) const override {
                for (int pos = key.chunks[depth] - 1; pos >= 0; pos--)
                    if (children[pos] != nullptr)
                        return children[pos]->maximum();

                return this->_parent->predecessor(key, depth - 1);
            }

            virtual uint16_t min_size() const override { return 49; }

            virtual uint16_t max_size() const override { return 256; }

            virtual node_type get_type() const override { return node_type::node_256_t; }

            virtual void debug() const override {
                std::cout << "Node 256, count: " << size() << std::endl;
                for (size_t i = 0; i < 256; i++) {
                    if (children[i] != nullptr)
                        std::cout << i << " | ";
                    else
                        std::cout << "--" << " | ";
                }
                std::cout << std::endl;
                for (size_t i = 0; i < 256; i++) {
                    if (children[i] != nullptr)
                        std::cout << i << ": " << children[i] << " | ";
                    else
                        std::cout << "--" << " | ";
                }
                std::cout << std::endl;
            }
        };

    private:
        size_t _M_count;

        // Dummy node of special type to mark the right end of the container
        // Necessary as the end marker for iterators
        _Dummy_Node *_M_dummy_node;

    public:
        // Current root node of the radix tree
        Node_ptr _M_root;

        // Default constructor
        Adaptive_radix_tree() {
            init();
        }

        Adaptive_radix_tree(const _Key_transform &key_transformer)
                : _M_key_transform(key_transformer) {
            init();
        }

        // Copy constructor
        Adaptive_radix_tree(const Adaptive_radix_tree &__x) {
            switch (__x._M_root->get_type()) {
                case node_type::node_4_t:
                    _M_root = new _Node_4(*static_cast<_Node_4 *>(__x._M_root));
                    break;
                case node_type::node_16_t:
                    _M_root = new _Node_16(*static_cast<_Node_16 *>(__x._M_root));
                    break;
                case node_type::node_48_t:
                    _M_root = new _Node_48(*static_cast<_Node_48 *>(__x._M_root));
                    break;
                case node_type::node_256_t:
                    _M_root = new _Node_256(*static_cast<_Node_256 *>(__x._M_root));
                    break;
                case node_type::_leaf_t:
                    _M_root = new _Leaf(*static_cast<_Leaf *>(__x._M_root));
                    break;
                default:
                    _M_root = nullptr;
                    break;
            }
            _M_count = __x._M_count;
            _M_dummy_node = new _Dummy_Node();
            if (_M_root != nullptr) {
                _M_root->_parent = _M_dummy_node;
                _M_dummy_node->_root = &_M_root;
            }

            _M_key_transform = __x._M_key_transform;
        }

        // Move constructor
        Adaptive_radix_tree(Adaptive_radix_tree &&__x) {
            if (__x._M_root != nullptr) {
                _M_root = std::move(__x._M_root);
            } else {
                _M_root = nullptr;
            }

            _M_count = std::move(__x._M_count);
            _M_key_transform = std::move(__x._M_key_transform);
            _M_dummy_node = new _Dummy_Node();
            if (_M_root != nullptr) {
                _M_root->_parent = _M_dummy_node;
                _M_dummy_node->_root = &_M_root;
            }

            __x._M_root = nullptr;
            __x._M_count = 0;
        }

        // Copy assignment
        Adaptive_radix_tree &operator=(const Adaptive_radix_tree &__x) {
            // remove old container contents
            clear();

            // Then do copy construction
            switch (__x._M_root->get_type()) {
                case node_type::node_4_t:
                    _M_root = new _Node_4(*static_cast<_Node_4 *>(__x._M_root));
                    break;
                case node_type::node_16_t:
                    _M_root = new _Node_16(*static_cast<_Node_16 *>(__x._M_root));
                    break;
                case node_type::node_48_t:
                    _M_root = new _Node_48(*static_cast<_Node_48 *>(__x._M_root));
                    break;
                case node_type::node_256_t:
                    _M_root = new _Node_256(*static_cast<_Node_256 *>(__x._M_root));
                    break;
                case node_type::_leaf_t:
                    _M_root = new _Leaf(*static_cast<_Leaf *>(__x._M_root));
                    break;
                default:
                    _M_root = nullptr;
                    break;
            }
            _M_count = __x._M_count;
            _M_dummy_node = new _Dummy_Node();
            if (_M_root != nullptr) {
                _M_root->_parent = _M_dummy_node;
                _M_dummy_node->_root = &_M_root;
            }
            _M_key_transform = __x._M_key_transform;
            return *this;
        }

        // Move assignment
        Adaptive_radix_tree &operator=(Adaptive_radix_tree &&__x) {
            _M_root = std::move(__x._M_root);
            _M_count = std::move(__x._M_count);
            _M_key_transform = std::move(__x._M_key_transform);

            if (_M_dummy_node == nullptr)
                _M_dummy_node = new _Dummy_Node();

            if (_M_root != nullptr) {
                _M_root->_parent = _M_dummy_node;
                _M_dummy_node->_root = &_M_root;
            }

            __x._M_root = nullptr;
            __x._M_count = 0;
            return *this;
        }

        void init() {
            _M_count = 0;
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
            typedef std::pair<const _Key, _V> value_type;
            typedef value_type &reference;
            typedef value_type *pointer;

            typedef std::bidirectional_iterator_tag iterator_category;
            typedef ptrdiff_t difference_type;

            typedef adapt_radix_tree_iterator _Self;
            typedef _Leaf *_Link_type;

            // pointer to current leaf node
            Node_ptr node;

            adapt_radix_tree_iterator() : node(nullptr) { }


            adapt_radix_tree_iterator(Node_ptr node)
                    : node(node) {
            }

            // preincrement
            _Self &operator++() {
                Key key = static_cast<_Link_type>(node)->key;
                int depth = static_cast<_Link_type>(node)->depth;
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
                Key key = static_cast<_Link_type>(node)->key;
                int depth = static_cast<_Link_type>(node)->depth;
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
            typedef std::pair<const _Key, _V> value_type;
            typedef const value_type &reference;
            typedef const value_type *pointer;

            typedef adapt_radix_tree_iterator iterator;

            typedef std::bidirectional_iterator_tag iterator_category;
            typedef ptrdiff_t difference_type;

            typedef adapt_radix_tree_const_iterator _Self;
            typedef const _Leaf *_Link_type;

            // pointer to current leaf node
            Const_Node_ptr node;

            adapt_radix_tree_const_iterator() : node(nullptr) { }

            // Copy Constructor
            adapt_radix_tree_const_iterator(const iterator &__it)
                    : node(__it.node) { }

            explicit adapt_radix_tree_const_iterator(Const_Node_ptr node)
                    : node(node) {
            }

            // preincrement
            _Self &operator++() {
                Key key = static_cast<_Link_type>(node)->key;
                int depth = static_cast<_Link_type>(node)->depth;
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
                Key key = static_cast<_Link_type>(node)->key;
                int depth = static_cast<_Link_type>(node)->depth;
                node = node->_parent->predecessor(key, depth - 1);
                return *this;
            }

            // postdecrement
            _Self operator--(int) {
                _Self retval = *this;
                --(*this);
                return retval;
            }

            iterator _const_cast() const {
                return iterator(const_cast<Node_ptr>(node));
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

        typedef adapt_radix_tree_iterator iterator;
        typedef adapt_radix_tree_const_iterator const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        iterator begin() {
            Node_ptr min = minimum();
            if (min == nullptr)
                return iterator(_M_dummy_node->_leaf);

            return iterator(min);
        }

        const_iterator begin() const {
            Const_Node_ptr min = minimum();
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
            Node_ptr min = minimum();
            if (min == nullptr)
                return reverse_iterator(iterator(_M_dummy_node->_leaf));
            return reverse_iterator(iterator(min));
        }

        const_reverse_iterator rend() const {
            Const_Node_ptr min = minimum();
            if (min == nullptr)
                return const_reverse_iterator(const_iterator(_M_dummy_node->_leaf));
            return const_reverse_iterator(const_iterator(min));
        }

        _Key_transform key_trans() const {
            return _M_key_transform;
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
            _M_root = nullptr;
            _M_count = 0;
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

            Node_ptr *current_node = &_M_root;
            Node_ptr *previous_node = nullptr;

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
                                Node_ptr *old_child;
                                old_child = (*current_node)->find(existing_key.chunks[j]);
                                Node_ptr new_child = new _Node_4(existing_leaf, j + 1);
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

        size_type erase_unique(const key_type &__k) {
            // Empty Tree
            if (_M_root == nullptr) {
                return 0;
            }

            Key transformed_key = {_M_key_transform(__k)};
            const auto key_size = sizeof(__k);

            if (_M_root->is_leaf()) {
                if (transformed_key.value == static_cast<_Leaf *>(_M_root)->key.value) {
                    delete _M_root;
                    _M_root = nullptr;
                    _M_count--;
                    return 1;
                }
                return 0;
            }

            Node_ptr *current_node = &_M_root;

            for (int i = 0; i < key_size + 1; i++) {
                if (current_node == nullptr || *current_node == nullptr)
                    return 0;

                Node_ptr *child = (*current_node)->find(transformed_key.chunks[i]);

                if (child == nullptr || *child == nullptr)
                    return 0;

                if ((*child)->is_leaf()) {
                    _Leaf *existing_leaf = static_cast<_Leaf *>(*child);
                    if (transformed_key.value == existing_leaf->key.value) {
                        // Delete the leaf
                        (*current_node)->erase(transformed_key.chunks[i]);
                        _M_count--;
                        fix_after_erase(transformed_key, current_node, i);
                        return 1;
                    } else {
                        //std::cout << "key not in tree (leaf mismatch)" << std::endl;
                        return 0; // key not in tree (leaf mismatch)
                    }
                } else {
                    current_node = child;
                }

            }
            throw; // unreachable
        }

        iterator erase(iterator __it) {
            iterator __result = __it;
            ++__result;

            // Delete leaf
            Key transformed_key = {_M_key_transform(__it->first)};
            _Leaf *l = static_cast<_Leaf *>(__it.node);
            Node_ptr *parent = &(l->_parent);
            int depth = l->depth - 1;
            (*parent)->erase(transformed_key.chunks[depth]);
            _M_count--;

            fix_after_erase(transformed_key, parent, depth);

            return __result;
        }

        /**
        const_iterator erase(const_iterator __it) {
            const_iterator __result = __it;
            ++__result;

            // Delete leaf
            Key transformed_key = {_M_key_transform(__it->first)};
            _Leaf *l = static_cast<_Leaf *>(__it.node);
            Node_ptr *parent = &(l->_parent);
            int depth = l->depth - 1;
            (*parent)->erase(transformed_key.chunks[depth]);
            _M_count--;

            fix_after_erase(transformed_key, parent, depth);

            return __result;
        }
         */

        void fix_after_erase(const Key &transformed_key, Node_ptr *node, int depth) {
            // Parent of deleted leaf now underfull?
            int j = 1;
            while ((*node)->size() < (*node)->min_size()) {
                std::pair<Node_ptr, bool> p = shrink(*node);

                // Cannot shrink node 4 to leaf, because child is not a leaf
                if (!p.second)
                    break;

                *node = p.first;

                // As long as the node above the leaf is a one-way node, compress path
                while ((*node)->is_leaf() && (*node)->_parent->size() <= 1 && depth - j >= -1) {

                    Node_ptr *parent = &((*node)->_parent);
                    if ((*parent)->get_type() == node_type::_dummy_node_t) {
                        (*node)->_parent = _M_dummy_node;
                        _M_root = *node;
                        return;
                    }

                    Node_ptr *grandparent = &((*parent)->_parent);
                    if ((*grandparent)->get_type() == node_type::_dummy_node_t) {
                        delete *parent;
                        (*node)->_parent = *grandparent;
                        _M_root = *node;
                        return;
                    }
                    Node_ptr *former_child = (*grandparent)->find(transformed_key.chunks[depth - j - 1]);
                    *former_child = *node;
                    delete *parent;
                    (*node)->_parent = *grandparent;
                    j++;
                }
            }
        }

        void swap(Adaptive_radix_tree &__x) {
            std::swap(_M_root, __x._M_root);
            std::swap(_M_count, __x._M_count);
            std::swap(_M_dummy_node, __x._M_dummy_node);
            std::swap(_M_key_transform, __x._M_key_transform);
        }

        ////////////
        // Lookup //
        ////////////

        iterator find(const key_type &__k) {
            Key transformed_key = {_M_key_transform(__k)};
            const auto key_size = sizeof(__k);

            if (_M_root == nullptr)
                return end();

            Node_ptr *current_node = &_M_root;
            for (unsigned i = 0; i < key_size + 1; i++) {
                if (current_node == nullptr)
                    return end();

                if ((*current_node)->is_leaf()) {
                    if (transformed_key.value == static_cast<_Leaf *>(*current_node)->key.value) {
                        return iterator(*current_node);
                    } else {
                        return end();
                    }
                }

                current_node = (*current_node)->find(transformed_key.chunks[i]);
            }
            throw; // unreachable
        }

        const_iterator find(const key_type &__k) const {
            Key transformed_key = {_M_key_transform(__k)};
            const auto key_size = sizeof(__k);

            if (_M_root == nullptr)
                return end();

            Const_Node_ptr current_node = _M_root;
            for (unsigned i = 0; i < key_size + 1; i++) {
                if (current_node == nullptr)
                    return end();

                if (current_node->is_leaf()) {
                    if (transformed_key.value == static_cast<const _Leaf *>(current_node)->key.value) {
                        return const_iterator(current_node);
                    } else {
                        return end();
                    }
                }

                current_node = current_node->find(transformed_key.chunks[i]);
            }
            throw; // unreachable
        }

        iterator lower_bound(const key_type &__k) {
            Key transformed_key = {_M_key_transform(__k)};
            const auto key_size = sizeof(__k);

            if (_M_root == nullptr)
                return end();

            Node_ptr *previous_node = nullptr;
            Node_ptr *current_node = &_M_root;
            for (unsigned i = 0; i < key_size + 1; i++) {
                if (current_node == nullptr || *current_node == nullptr) {
                    auto successor = (*previous_node)->successor(transformed_key, i - 1);
                    return iterator(successor);
                }

                if ((*current_node)->is_leaf()) {
                    if (transformed_key.value <= static_cast<_Leaf *>(*current_node)->key.value) {
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

        const_iterator lower_bound(const key_type &__k) const {
            Key transformed_key = {_M_key_transform(__k)};
            const auto key_size = sizeof(__k);

            if (_M_root == nullptr)
                return end();

            Node_ptr *previous_node = nullptr;
            Node_ptr *current_node = &_M_root;
            for (unsigned i = 0; i < key_size + 1; i++) {
                if (current_node == nullptr || *current_node == nullptr) {
                    auto successor = (*previous_node)->successor(transformed_key, i - 1);
                    return const_iterator(successor);
                }

                if ((*current_node)->is_leaf()) {
                    if (transformed_key.value <= static_cast<_Leaf *>(*current_node)->key.value) {
                        return const_iterator(*current_node);
                    } else {
                        auto successor = (*previous_node)->successor(transformed_key, i - 1);
                        return const_iterator(successor);
                    }
                }

                previous_node = current_node;
                current_node = (*current_node)->find(transformed_key.chunks[i]);
            }
            throw; // unreachable
        }

        iterator upper_bound(const key_type &__k) {
            Key transformed_key = {_M_key_transform(__k)};
            const auto key_size = sizeof(__k);

            if (_M_root == nullptr)
                return end();

            Node_ptr *previous_node = nullptr;
            Node_ptr *current_node = &_M_root;
            for (unsigned i = 0; i < key_size + 1; i++) {
                if (current_node == nullptr || *current_node == nullptr) {
                    auto successor = (*previous_node)->successor(transformed_key, i - 1);
                    return iterator(successor);
                }

                if ((*current_node)->is_leaf()) {
                    if (transformed_key.value < static_cast<_Leaf *>(*current_node)->key.value) {
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

        const_iterator upper_bound(const key_type &__k) const {
            Key transformed_key = {_M_key_transform(__k)};
            const auto key_size = sizeof(__k);

            if (_M_root == nullptr)
                return end();

            Node_ptr *previous_node = nullptr;
            Node_ptr *current_node = &_M_root;
            for (unsigned i = 0; i < key_size + 1; i++) {
                if (current_node == nullptr || *current_node == nullptr) {
                    auto successor = (*previous_node)->successor(transformed_key, i - 1);
                    return const_iterator(successor);
                }

                if ((*current_node)->is_leaf()) {
                    if (transformed_key.value < static_cast<_Leaf *>(*current_node)->key.value) {
                        return const_iterator(*current_node);
                    } else {
                        auto successor = (*previous_node)->successor(transformed_key, i - 1);
                        return const_iterator(successor);
                    }
                }

                previous_node = current_node;
                current_node = (*current_node)->find(transformed_key.chunks[i]);
            }
            throw; // unreachable
        }

        std::pair<iterator, iterator> equal_range(const key_type &__k) {
            auto lower = lower_bound(__k);
            auto upper = upper_bound(__k);
            return std::pair<iterator, iterator>(lower, upper);
        };

        std::pair<const_iterator, const_iterator> equal_range(const key_type &__k) const {
            auto lower = lower_bound(__k);
            auto upper = upper_bound(__k);
            return std::pair<const_iterator, const_iterator>(lower, upper);
        };

        Node_ptr minimum() {
            if (_M_root != nullptr)
                return _M_root->minimum();
            return nullptr;
        }

        Const_Node_ptr minimum() const {
            if (_M_root != nullptr)
                return _M_root->minimum();
            return nullptr;
        }

        Node_ptr maximum() {
            if (_M_root != nullptr)
                return _M_root->maximum();
            return nullptr;
        }

        Const_Node_ptr maximum() const {
            if (_M_root != nullptr)
                return _M_root->maximum();
            return nullptr;
        }

        ~Adaptive_radix_tree() {
            clear();
            delete _M_dummy_node;
        }

    private:
        Node_ptr grow(Node_ptr old_node) {
            auto type = old_node->get_type();
            switch (type) {
                case node_type::node_4_t: {
                    _Node_4 *node4 = static_cast<_Node_4 *>(old_node);
                    Node_ptr node = new _Node_16(node4);
                    delete node4;
                    return node;
                }
                case node_type::node_16_t: {
                    _Node_16 *node16 = static_cast<_Node_16 *>(old_node);
                    Node_ptr node = new _Node_48(node16);
                    delete node16;
                    return node;
                }
                case node_type::node_48_t: {
                    _Node_48 *node48 = static_cast<_Node_48 *>(old_node);
                    Node_ptr node = new _Node_256(node48);
                    delete node48;
                    return node;
                }
            }
            throw; // unreachable
        }

        std::pair<Node_ptr, bool> shrink(Node_ptr old_node) {
            auto type = old_node->get_type();
            switch (type) {
                case node_type::node_4_t: {
                    _Node_4 *node4 = static_cast<_Node_4 *>(old_node);

                    if (node4->children[0]->is_leaf()) {
                        node4->children[0]->_parent = old_node->_parent;
                        delete node4;
                        return {node4->children[0], true};
                    }
                    return {old_node, false};
                }
                case node_type::node_16_t: {
                    _Node_16 *node16 = static_cast<_Node_16 *>(old_node);
                    Node_ptr node = new _Node_4(node16);
                    delete node16;
                    return {node, true};
                }
                case node_type::node_48_t: {
                    _Node_48 *node48 = static_cast<_Node_48 *>(old_node);
                    Node_ptr node = new _Node_16(node48);
                    delete node48;
                    return {node, true};
                }
                case node_type::node_256_t: {
                    _Node_256 *node256 = static_cast<_Node_256 *>(old_node);
                    Node_ptr node = new _Node_48(node256);
                    delete node256;
                    return {node, true};
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
