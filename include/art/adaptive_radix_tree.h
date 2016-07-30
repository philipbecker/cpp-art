#ifndef ART_ADAPT_RADIX_TREE_H
#define ART_ADAPT_RADIX_TREE_H

#include <array>
#include <stddef.h>
#include <iterator>
#include <utility>
#include <iostream>
#include <limits>
#include "key_transform.h"

namespace art
{
    using std::pair;
    using std::make_pair;

    typedef uint8_t byte;
    static const byte EMPTY_MARKER = 63;

    template<typename _Key, typename _Value, typename _KeyOfValue,
            typename _Key_transform = key_transform<_Key> >
    struct adaptive_radix_tree {
    public:
        // Forward declaration for typedefs
        struct _Node;

        struct _Inner_Node;

        struct _Leaf;

        struct _Node_4;

        struct _Node_16;

        struct _Node_48;

        struct _Node_256;

        typedef _Key key_type;
        typedef _Value value_type;

    private:
        typedef _Node *Node_ptr;
        typedef const _Node *Const_Node_ptr;
        typedef _Inner_Node *Inner_Node_ptr;
        typedef const _Inner_Node *Const_Inner_Node_ptr;
        typedef _Leaf *Leaf_ptr;
        typedef const _Leaf *Const_Leaf_ptr;

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

        struct _Node {
        public:
            Node_ptr _parent;

            _Node(Node_ptr parent) : _parent(parent) {}

            // Copy constructor
            _Node(const _Node &__x) : _parent(__x._parent) {
            }

            // Copy assignment
            _Node &operator=(const _Node &__x) {
                _parent = __x._parent;
                return *this;
            }

            virtual ~_Node() {}

            virtual void clear() = 0;

            virtual void insert(const byte &key_byte, Node_ptr node) = 0;

            virtual void erase(const byte &key_byte) = 0;

            virtual Node_ptr *find_ref(const byte &key_byte) = 0;

            virtual Node_ptr find(const byte &key_byte) = 0;

            virtual Const_Node_ptr find(const byte &key_byte) const = 0;

            virtual void update_child_ptr(const byte &key_byte, Node_ptr node) = 0;

            virtual Node_ptr minimum() = 0;

            virtual Const_Node_ptr minimum() const = 0;

            virtual Node_ptr maximum() = 0;

            virtual Const_Node_ptr maximum() const = 0;

            virtual bool is_leaf() const { return false; }

            virtual uint16_t size() const { return 1; }

            virtual uint16_t min_size() const = 0;

            virtual uint16_t max_size() const = 0;

            virtual node_type get_type() const = 0;

            virtual Node_ptr successor(const Key &key) = 0;

            virtual Const_Node_ptr successor(const Key &key) const = 0;

            virtual Node_ptr predecessor(const Key &key) = 0;

            virtual Const_Node_ptr predecessor(const Key &key) const = 0;

            virtual void debug() const = 0;
        };

        struct _Inner_Node : public _Node {
        public:
            uint16_t _count;

            int32_t _depth;

            _Inner_Node(Node_ptr parent, uint16_t count, int32_t depth)
                    : _Node(parent), _count(count), _depth(depth) {}


            // Copy constructor
            _Inner_Node(const _Inner_Node &__x)
                    : _Node(__x._parent), _count(__x._count), _depth(__x._depth) {
            }

            // Copy assignment
            _Inner_Node &operator=(const _Inner_Node &__x) {
                this->_parent = __x._parent;
                _count = __x._count;
                _depth = __x._depth;
                return *this;
            }

            virtual void clear() = 0;

            virtual void insert(const byte &key_byte, Node_ptr node) = 0;

            virtual void erase(const byte &key_byte) = 0;

            virtual Node_ptr *find_ref(const byte &key_byte) = 0;

            virtual Node_ptr find(const byte &key_byte) = 0;

            virtual Const_Node_ptr find(const byte &key_byte) const = 0;

            virtual void update_child_ptr(const byte &key_byte, Node_ptr node) = 0;

            virtual Node_ptr minimum() = 0;

            virtual Const_Node_ptr minimum() const = 0;

            virtual Node_ptr maximum() = 0;

            virtual Const_Node_ptr maximum() const = 0;

            virtual bool is_leaf() const { return false; }

            virtual uint16_t size() const { return _count; }

            virtual uint16_t min_size() const = 0;

            virtual uint16_t max_size() const = 0;

            virtual node_type get_type() const = 0;

            virtual Node_ptr successor(const Key &key) = 0;

            virtual Const_Node_ptr successor(const Key &key) const = 0;

            virtual Node_ptr predecessor(const Key &key) = 0;

            virtual Const_Node_ptr predecessor(const Key &key) const = 0;

            virtual void debug() const = 0;
        };

        struct _Leaf : public _Node {
        public:
            value_type _value;

            _Leaf(value_type value)
                    : _Node(nullptr), _value(value) {}


            _Leaf(value_type value, Node_ptr parent)
                    : _Node(parent), _value(value) {}

            // Copy constructor
            _Leaf(const _Leaf &__x) : _Node(nullptr), _value(__x._value) {}

            // Copy assignment
            _Leaf &operator=(const _Leaf &__x) {
                this->_parent = nullptr;
                _value = __x._value;
                return *this;
            }

            void clear() override {}

            virtual void insert(const byte &key_byte, Node_ptr node) override {}

            virtual void erase(const byte &key_byte) override {}

            virtual Node_ptr *find_ref(const byte &key_byte) override {
                return nullptr;
            }

            virtual Node_ptr find(const byte &key_byte) override {
                return nullptr;
            }

            virtual Const_Node_ptr find(const byte &key_byte) const override {
                return nullptr;
            }

            virtual void update_child_ptr(const byte &key_byte, Node_ptr node) override {}

            virtual Node_ptr minimum() override { return this; }

            virtual Const_Node_ptr minimum() const override { return this; }

            virtual Node_ptr maximum() override { return this; }

            virtual Const_Node_ptr maximum() const override { return this; }

            virtual Node_ptr successor(const Key &key) override { return this; }

            virtual Const_Node_ptr successor(const Key &key) const override { return this; }

            virtual Node_ptr predecessor(const Key &key) override { return this; }

            virtual Const_Node_ptr predecessor(const Key &key) const override { return this; }

            virtual uint16_t min_size() const override { return 1; }

            virtual uint16_t max_size() const override { return 1; }

            bool is_leaf() const override { return true; }

            virtual node_type get_type() const override { return node_type::_leaf_t; }

            virtual void debug() const override {
                std::cout << this << " Leaf: "
                          << ", parent " << this->_parent << std::endl;
            }
        };

        /**
         * End of iteration marker right of the container.
         */
        struct _Dummy_Node : public _Node {
        public:
            Node_ptr _root;

            Leaf_ptr _leaf;

            _Dummy_Node()
                    : _Node(this), _root(nullptr),
                      _leaf(new _Leaf(value_type(), this)) {
            }

            _Dummy_Node(Node_ptr root, Leaf_ptr leaf)
                    : _Node(this), _root(root),
                      _leaf(new _Leaf(value_type(), this)) {
            }

            // Copy constructor
            _Dummy_Node(const _Dummy_Node &__x) : _Node(__x._parent) {
                _leaf = __x._leaf;
            }

            // Copy assignment
            _Dummy_Node &operator=(const _Dummy_Node &__x) {
                this->_parent = __x._parent;
                _leaf = __x._leaf;
                return *this;
            }

            void clear() override {}

            virtual void insert(const byte &key_byte, Node_ptr node) override {}

            virtual void erase(const byte &key_byte) override {}

            virtual Node_ptr *find_ref(const byte &key_byte) override {
                return nullptr;
            }

            virtual Node_ptr find(const byte &key_byte) override {
                return nullptr;
            }

            virtual Const_Node_ptr find(const byte &key_byte) const override {
                return nullptr;
            }

            virtual void update_child_ptr(const byte &key_byte, Node_ptr node) override {
                _root = node;
            }

            virtual Node_ptr minimum() override { return this; }

            virtual Const_Node_ptr minimum() const override { return this; }

            virtual Node_ptr maximum() override { return this; }

            virtual Const_Node_ptr maximum() const override { return this; }

            virtual Node_ptr successor(const Key &key) override {
                return this->_leaf;
            }

            virtual Const_Node_ptr successor(const Key &key) const override {
                return this->_leaf;
            }

            virtual Node_ptr predecessor(const Key &key) override {
                if (_root != nullptr)
                    return _root->maximum();

                return this->_leaf;
            }

            virtual Const_Node_ptr predecessor(const Key &key) const override {
                if (_root != nullptr)
                    return _root->maximum();

                return this->_leaf;
            }

            virtual uint16_t min_size() const override { return 0; }

            virtual uint16_t max_size() const override { return 0; }

            virtual node_type get_type() const override { return node_type::_dummy_node_t; }

            virtual void debug() const override {
                std::cout << "Dummy Node debug" << std::endl;
            }

            ~_Dummy_Node() {
                delete _leaf;
            }
        };

        struct _Node_4 : public _Inner_Node {
        public:
            std::array<byte, 4> keys{};
            std::array<Node_ptr, 4> children{};

            // Grow constructor
            _Node_4(Leaf_ptr leaf, const byte &key_byte, int32_t depth)
                    : _Inner_Node(leaf->_parent, 1, depth) {
                keys[0] = key_byte;
                children[0] = leaf;
                leaf->_parent = this;
            }

            // Shrink constructor
            _Node_4(_Node_16 *node)
                    : _Inner_Node(node->_parent, 4, node->_depth) {
                std::copy(node->keys.begin(), node->keys.begin() + 4, keys.begin());
                std::copy(node->children.begin(), node->children.begin() + 4, children.begin());

                for (size_t i = 0; i < 4; i++)
                    children[i]->_parent = this;
            }

            // Copy constructor
            _Node_4(const _Node_4 &__x)
                    : _Inner_Node(__x._parent, __x._count, __x._depth), keys(__x.keys) {
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
                            children[i] = new _Leaf(*static_cast<Leaf_ptr>(__x.children[i]));
                            children[i]->_parent = this;
                            break;
                        default:
                            throw;
                    }
                }
            }

            // Copy assignment
            _Node_4 &operator=(const _Node_4 &__x) {
                this->_count = __x._count;
                this->_parent = __x._parent;
                this->_depth = __x._depth;
                keys = __x.keys;
                children = __x.children;
                return *this;
            }

            void clear() override {
                for (size_t i = 0; i < this->_count; i++) {
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

            virtual Node_ptr *find_ref(const byte &key_byte) override {
                unsigned pos = 0;
                for (; pos < this->_count && keys[pos] < key_byte; pos++);

                if (pos < this->_count && keys[pos] == key_byte)
                    return &children[pos];
                return nullptr;
            }

            virtual Node_ptr find(const byte &key_byte) override {
                unsigned pos = 0;
                for (; pos < this->_count && keys[pos] < key_byte; pos++);

                if (pos < this->_count && keys[pos] == key_byte)
                    return children[pos];
                return nullptr;
            }

            virtual Const_Node_ptr find(const byte &key_byte) const override {
                unsigned pos = 0;
                for (; pos < this->_count && keys[pos] < key_byte; pos++);

                if (pos < this->_count && keys[pos] == key_byte)
                    return children[pos];
                return nullptr;
            }

            virtual void update_child_ptr(const byte &key_byte, Node_ptr node) override {
                unsigned pos = 0;
                for (; pos < this->_count && keys[pos] < key_byte; pos++);

                if (pos < this->_count && keys[pos] == key_byte)
                    children[pos] = node;
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

            virtual Node_ptr successor(const Key &key) override {
                unsigned pos = 0;
                for (; pos < this->_count && keys[pos] <= key.chunks[this->_depth]; pos++);
                if (pos < this->_count)
                    return children[pos]->minimum();
                else
                    return this->_parent->successor(key);
            }

            virtual Const_Node_ptr successor(const Key &key) const override {
                unsigned pos = 0;
                for (; pos < this->_count && keys[pos] <= key.chunks[this->_depth]; pos++);
                if (pos < this->_count)
                    return children[pos]->minimum();
                else
                    return this->_parent->successor(key);
            }

            virtual Node_ptr predecessor(const Key &key) override {
                int pos = this->_count - 1;
                for (; pos >= 0 && keys[pos] >= key.chunks[this->_depth]; pos--);
                if (pos >= 0)
                    return children[pos]->maximum();
                else
                    return this->_parent->predecessor(key);
            }

            virtual Const_Node_ptr predecessor(const Key &key) const override {
                int pos = this->_count - 1;
                for (; pos >= 0 && keys[pos] >= key.chunks[this->_depth]; pos--);
                if (pos >= 0)
                    return children[pos]->maximum();
                else
                    return this->_parent->predecessor(key);
            }

            virtual uint16_t min_size() const override { return 2; }

            virtual uint16_t max_size() const override { return 4; }

            virtual node_type get_type() const override { return node_type::node_4_t; }

            virtual void debug() const override {
                std::cout << this << " Node 4, count: " << this->size()
                          << ", parent " << this->_parent << std::endl;
                if (this->size() > 0) {
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

        struct _Node_16 : public _Inner_Node {
        public:
            std::array<byte, 16> keys{};
            std::array<Node_ptr, 16> children{};

            // Grow constructor
            _Node_16(_Node_4 *node)
                    : _Inner_Node(node->_parent, 4, node->_depth) {
                std::copy(node->keys.begin(), node->keys.end(), keys.begin());
                std::copy(node->children.begin(), node->children.end(), children.begin());

                for (size_t i = 0; i < 4; i++)
                    children[i]->_parent = this;
            }

            // Shrink constructor
            _Node_16(_Node_48 *node)
                    : _Inner_Node(node->_parent, 16, node->_depth) {
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
            _Node_16(const _Node_16 &__x)
                    : _Inner_Node(__x._parent, __x._count, __x._depth), keys(__x.keys) {
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
                            children[i] = new _Leaf(*static_cast<Leaf_ptr>(__x.children[i]));
                            children[i]->_parent = this;
                            break;
                        default:
                            throw;
                            break;
                    }
                }
            }

            // Copy assignment
            _Node_16 &operator=(const _Node_16 &__x) {
                this->_count = __x._count;
                this->_parent = __x._parent;
                this->_depth = __x._depth;
                keys = __x.keys;
                children = __x.children;
                return *this;
            }

            void clear() override {
                for (size_t i = 0; i < this->_count; i++) {
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

            virtual Node_ptr *find_ref(const byte &key_byte) override {
                unsigned pos = 0;
                for (; pos < this->_count && keys[pos] < key_byte; pos++);

                if (pos < this->_count && keys[pos] == key_byte)
                    return &children[pos];
                return nullptr;
            }

            virtual Node_ptr find(const byte &key_byte) override {
                unsigned pos = 0;
                for (; pos < this->_count && keys[pos] < key_byte; pos++);

                if (pos < this->_count && keys[pos] == key_byte)
                    return children[pos];
                return nullptr;
            }

            virtual Const_Node_ptr find(const byte &key_byte) const override {
                unsigned pos = 0;
                for (; pos < this->_count && keys[pos] < key_byte; pos++);

                if (pos < this->_count && keys[pos] == key_byte)
                    return children[pos];
                return nullptr;
            }

            virtual void update_child_ptr(const byte &key_byte, Node_ptr node) override {
                unsigned pos = 0;
                for (; pos < this->_count && keys[pos] < key_byte; pos++);

                if (pos < this->_count && keys[pos] == key_byte)
                    children[pos] = node;
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

            virtual Node_ptr successor(const Key &key) override {
                unsigned pos = 0;
                for (; pos < this->_count && keys[pos] <= key.chunks[this->_depth]; pos++);
                if (pos < this->_count)
                    return children[pos]->minimum();
                else
                    return this->_parent->successor(key);
            }

            virtual Const_Node_ptr successor(const Key &key) const override {
                unsigned pos = 0;
                for (; pos < this->_count && keys[pos] <= key.chunks[this->_depth]; pos++);
                if (pos < this->_count)
                    return children[pos]->minimum();
                else
                    return this->_parent->successor(key);
            }

            virtual Node_ptr predecessor(const Key &key) override {
                int pos = this->_count - 1;
                for (; pos >= 0 && keys[pos] >= key.chunks[this->_depth]; pos--);
                if (pos >= 0)
                    return children[pos]->maximum();
                else
                    return this->_parent->predecessor(key);
            }

            virtual Const_Node_ptr predecessor(const Key &key) const override {
                int pos = this->_count - 1;
                for (; pos >= 0 && keys[pos] >= key.chunks[this->_depth]; pos--);
                if (pos >= 0)
                    return children[pos]->maximum();
                else
                    return this->_parent->predecessor(key);
            }

            virtual uint16_t min_size() const override { return 5; }

            virtual uint16_t max_size() const override { return 16; }

            virtual node_type get_type() const override { return node_type::node_16_t; }

            virtual void debug() const override {
                std::cout << this << " Node 16, count: " << this->size()
                          << ", parent " << this->_parent << std::endl;
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


        struct _Node_48 : public _Inner_Node {
        public:
            std::array<byte, 256> child_index;
            std::array<Node_ptr, 48> children{};

            // Grow constructor
            _Node_48(_Node_16 *node)
                    : _Inner_Node(node->_parent, 16, node->_depth) {
                std::fill(child_index.begin(), child_index.end(), EMPTY_MARKER);

                for (uint8_t i = 0; i < 16; i++) {
                    child_index[node->keys[i]] = i;
                    children[i] = node->children[i];
                    children[i]->_parent = this;
                }
            }

            // Shrink constructor
            _Node_48(_Node_256 *node)
                    : _Inner_Node(node->_parent, 48, node->_depth) {
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
            _Node_48(const _Node_48 &__x)
                    : _Inner_Node(__x._parent, __x._count, __x._depth), child_index(__x.child_index) {
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
                                children[child_index[i]] = new _Leaf(*static_cast<Leaf_ptr>(__x.children[i]));
                                children[child_index[i]]->_parent = this;
                                break;
                            default:
                                throw;
                                break;
                        }
                    }
                }
            }

            // Copy assignment
            _Node_48 &operator=(const _Node_48 &__x) {
                this->_count = __x._count;
                this->_parent = __x._parent;
                this->_depth = __x._depth;
                child_index = __x.child_index;
                children = __x.children;
                return *this;
            }

            void clear() override {
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

            virtual Node_ptr *find_ref(const byte &key_byte) override {
                if (child_index[key_byte] != EMPTY_MARKER)
                    return &children[child_index[key_byte]];
                return nullptr;
            }

            virtual Node_ptr find(const byte &key_byte) override {
                if (child_index[key_byte] != EMPTY_MARKER)
                    return children[child_index[key_byte]];
                return nullptr;
            }

            virtual Const_Node_ptr find(const byte &key_byte) const override {
                if (child_index[key_byte] != EMPTY_MARKER)
                    return children[child_index[key_byte]];
                return nullptr;
            }

            virtual void update_child_ptr(const byte &key_byte, Node_ptr node) override {
                if (child_index[key_byte] != EMPTY_MARKER)
                    children[child_index[key_byte]] = node;
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

            virtual Node_ptr successor(const Key &key) override {
                for (unsigned pos = ((unsigned) key.chunks[this->_depth]) + 1; pos < 256; pos++)
                    if (child_index[pos] != EMPTY_MARKER)
                        return children[child_index[pos]]->minimum();

                return this->_parent->successor(key);
            }

            virtual Const_Node_ptr successor(const Key &key) const override {
                for (unsigned pos = ((unsigned) key.chunks[this->_depth]) + 1; pos < 256; pos++)
                    if (child_index[pos] != EMPTY_MARKER)
                        return children[child_index[pos]]->minimum();

                return this->_parent->successor(key);
            }

            virtual Node_ptr predecessor(const Key &key) override {
                for (int pos = ((int) key.chunks[this->_depth]) - 1; pos >= 0; pos--)
                    if (child_index[pos] != EMPTY_MARKER)
                        return children[child_index[pos]]->maximum();

                return this->_parent->predecessor(key);
            }

            virtual Const_Node_ptr predecessor(const Key &key) const override {
                for (int pos = ((int) key.chunks[this->_depth]) - 1; pos >= 0; pos--)
                    if (child_index[pos] != EMPTY_MARKER)
                        return children[child_index[pos]]->maximum();

                return this->_parent->predecessor(key);
            }

            virtual uint16_t min_size() const override { return 17; }

            virtual uint16_t max_size() const override { return 48; }

            virtual node_type get_type() const override { return node_type::node_48_t; }

            virtual void debug() const override {
                std::cout << this << " Node 48, count: " << this->size()
                          << ", parent " << this->_parent << std::endl;
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

        struct _Node_256 : public _Inner_Node {
        public:
            std::array<Node_ptr, 256> children{};

            // Grow constructor
            _Node_256(_Node_48 *node)
                    : _Inner_Node(node->_parent, 48, node->_depth) {
                for (uint16_t i = 0; i < 256; i++) {
                    if (node->child_index[i] != EMPTY_MARKER) {
                        children[i] = node->children[node->child_index[i]];
                        children[i]->_parent = this;
                    }
                }
            }

            // Copy constructor
            _Node_256(const _Node_256 &__x)
                    : _Inner_Node(__x._parent, __x._count, __x._depth) {
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
                                children[i] = new _Leaf(*static_cast<Leaf_ptr>(__x.children[i]));
                                children[i]->_parent = this;
                                break;
                            default:
                                throw;
                                break;
                        }
                    }
                }
            }

            // Copy assignment
            _Node_256 &operator=(const _Node_256 &__x) {
                this->_count = __x._count;
                this->_parent = __x._parent;
                return *this;
            }

            void clear() override {
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

            virtual Node_ptr *find_ref(const byte &key_byte) override {
                if (children[key_byte] != nullptr)
                    return &children[key_byte];
                return nullptr;
            }

            virtual Node_ptr find(const byte &key_byte) override {
                if (children[key_byte] != nullptr)
                    return children[key_byte];
                return nullptr;
            }

            virtual Const_Node_ptr find(const byte &key_byte) const override {
                if (children[key_byte] != nullptr)
                    return children[key_byte];
                return nullptr;
            }

            virtual void update_child_ptr(const byte &key_byte, Node_ptr node) override {
                if (children[key_byte] != nullptr)
                    children[key_byte] = node;
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

            virtual Node_ptr successor(const Key &key) override {
                for (unsigned pos = ((unsigned) key.chunks[this->_depth]) + 1; pos < 256; pos++)
                    if (children[pos] != nullptr)
                        return children[pos]->minimum();

                return this->_parent->successor(key);
            }

            virtual Const_Node_ptr successor(const Key &key) const override {
                for (unsigned pos = ((unsigned) key.chunks[this->_depth]) + 1; pos < 256; pos++)
                    if (children[pos] != nullptr)
                        return children[pos]->minimum();

                return this->_parent->successor(key);
            }

            virtual Node_ptr predecessor(const Key &key) override {
                for (int pos = key.chunks[this->_depth] - 1; pos >= 0; pos--)
                    if (children[pos] != nullptr)
                        return children[pos]->maximum();

                return this->_parent->predecessor(key);
            }

            virtual Const_Node_ptr predecessor(const Key &key) const override {
                for (int pos = key.chunks[this->_depth] - 1; pos >= 0; pos--)
                    if (children[pos] != nullptr)
                        return children[pos]->maximum();

                return this->_parent->predecessor(key);
            }

            virtual uint16_t min_size() const override { return 49; }

            virtual uint16_t max_size() const override { return 256; }

            virtual node_type get_type() const override { return node_type::node_256_t; }

            virtual void debug() const override {
                std::cout << this << " Node 256, count: " << this->size()
                          << ", parent " << this->_parent << std::endl;
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
        adaptive_radix_tree() {
            init();
        }

        adaptive_radix_tree(const _Key_transform &key_transformer)
                : _M_key_transform(key_transformer) {
            init();
        }

        // Copy constructor
        adaptive_radix_tree(const adaptive_radix_tree &__x) {
            if (__x.empty()) {
                // Nothing to copy
                init();
                return;
            }

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
                    _M_root = new _Leaf(*static_cast<Leaf_ptr>(__x._M_root));
                    break;
                default:
                    throw;
            }
            _M_count = __x._M_count;
            _M_dummy_node = new _Dummy_Node();
            if (_M_root != nullptr) {
                _M_root->_parent = _M_dummy_node;
                _M_dummy_node->_root = _M_root;
            }

            _M_key_transform = __x._M_key_transform;
        }

        // Move constructor
        adaptive_radix_tree(adaptive_radix_tree &&__x) {
            _M_root = std::move(__x._M_root);
            _M_count = std::move(__x._M_count);
            _M_key_transform = std::move(__x._M_key_transform);

            _M_dummy_node = new _Dummy_Node();
            if (_M_root != nullptr) {
                _M_root->_parent = _M_dummy_node;
                _M_dummy_node->_root = _M_root;
            }

            // Leaf move source in a valid state
            __x._M_root = nullptr;
            __x._M_count = 0;
        }

        // Copy assignment
        adaptive_radix_tree &operator=(const adaptive_radix_tree &__x) {
            if (this != &__x) {
                // remove old container contents
                clear();

                if (__x.empty()) {
                    // Nothing to copy
                    init();
                    return *this;
                }

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
                        _M_root = new _Leaf(*static_cast<Leaf_ptr>(__x._M_root));
                        break;
                    default:
                        throw;
                }
                _M_count = __x._M_count;
                if (_M_root != nullptr) {
                    _M_root->_parent = _M_dummy_node;
                    _M_dummy_node->_root = _M_root;
                }
                _M_key_transform = __x._M_key_transform;
            }
            return *this;
        }

        // Move assignment
        adaptive_radix_tree &operator=(adaptive_radix_tree &&__x) {
            _M_root = std::move(__x._M_root);
            _M_count = std::move(__x._M_count);
            _M_key_transform = std::move(__x._M_key_transform);

            if (_M_root != nullptr) {
                _M_root->_parent = _M_dummy_node;
                _M_dummy_node->_root = _M_root;
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

        size_t max_size() const {
            if (sizeof(transformed_key_type) < sizeof(size_type))
                return size_type(1) << (8 * sizeof(transformed_key_type));
            return std::numeric_limits<size_type>::max();
        }

        bool empty() const {
            return _M_count == 0;
        }

        ///////////////
        // Iterators //
        ///////////////

        struct adapt_radix_tree_iterator {
            typedef _Value value_type;
            typedef value_type &reference;
            typedef value_type *pointer;

            typedef std::bidirectional_iterator_tag iterator_category;
            typedef ptrdiff_t difference_type;

            typedef adapt_radix_tree_iterator _Self;

            // pointer to current leaf node
            Node_ptr node;

            adapt_radix_tree_iterator() : node(nullptr) {}


            adapt_radix_tree_iterator(Node_ptr node)
                    : node(node) {
            }

            // preincrement
            _Self &operator++() {
                Leaf_ptr leaf = static_cast<Leaf_ptr>(node);
                Inner_Node_ptr parent = static_cast<Inner_Node_ptr>(leaf->_parent);
                node = parent->successor({_Key_transform()(_KeyOfValue()(leaf->_value))});
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
                Leaf_ptr leaf = static_cast<Leaf_ptr>(node);
                Inner_Node_ptr parent = static_cast<Inner_Node_ptr>(leaf->_parent);
                node = parent->predecessor({_Key_transform()(_KeyOfValue()(leaf->_value))});
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
                return static_cast<Leaf_ptr>(node)->_value;
            }

            pointer operator->() const {
                return &static_cast<Leaf_ptr>(node)->_value;
            }
        };

        struct adapt_radix_tree_const_iterator {
            typedef _Value value_type;
            typedef const value_type &reference;
            typedef const value_type *pointer;

            typedef adapt_radix_tree_iterator iterator;

            typedef std::bidirectional_iterator_tag iterator_category;
            typedef ptrdiff_t difference_type;

            typedef adapt_radix_tree_const_iterator _Self;

            // pointer to current leaf node
            Const_Node_ptr node;

            adapt_radix_tree_const_iterator() : node(nullptr) {}

            // Copy Constructor
            adapt_radix_tree_const_iterator(const iterator &__it)
                    : node(__it.node) {}

            explicit adapt_radix_tree_const_iterator(Const_Node_ptr node)
                    : node(node) {
            }

            // preincrement
            _Self &operator++() {
                Const_Leaf_ptr leaf = static_cast<Const_Leaf_ptr>(node);
                Const_Inner_Node_ptr parent = static_cast<Const_Inner_Node_ptr>(leaf->_parent);
                node = parent->successor({_Key_transform()(_KeyOfValue()(leaf->_value))});
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
                Const_Leaf_ptr leaf = static_cast<Const_Leaf_ptr>(node);
                Const_Inner_Node_ptr parent = static_cast<Const_Inner_Node_ptr>(leaf->_parent);
                node = parent->predecessor({_Key_transform()(_KeyOfValue()(leaf->_value))});
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
                return static_cast<Const_Leaf_ptr>(node)->_value;
            }

            pointer operator->() const {
                return &static_cast<Const_Leaf_ptr>(node)->_value;
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
            return reverse_iterator(begin());
        }

        const_reverse_iterator rend() const {
            return const_reverse_iterator(begin());
        }

        _Key_transform key_trans() const {
            return _M_key_transform;
        }

        ///////////////
        // Modifiers //
        ///////////////

        void clear() {
            if (_M_root != nullptr) {
                _M_root->clear();
                delete _M_root;
            }
            _M_root = nullptr;
            _M_count = 0;
        }

        pair<iterator, bool> insert_unique(const value_type &__x) {
            Key transformed_key = {_M_key_transform(_KeyOfValue()(__x))};
            const auto key_size = sizeof(Key);

            // Empty Tree
            if (_M_root == nullptr) {
                Leaf_ptr new_leaf = new _Leaf(__x, _M_dummy_node);
                _M_root = new_leaf;
                _M_count++;

                _M_dummy_node->_root = _M_root;
                return make_pair(iterator(_M_root), true);
            }

            Node_ptr current_node = _M_root;
            Node_ptr previous_node = _M_dummy_node;

            for (unsigned i = 0; i < key_size + 1; i++) {
                if (current_node != nullptr) {
                    if (current_node->is_leaf()) {
                        // Hit an existing leaf
                        Leaf_ptr existing_leaf = reinterpret_cast<Leaf_ptr>(current_node);
                        Key existing_key = {_M_key_transform(_KeyOfValue()(existing_leaf->_value))};
                        if (transformed_key.value == existing_key.value) {
                            // if it is a duplicate entry, ignore
                            return make_pair(iterator(existing_leaf), false);
                        } else {
                            // otherwise, the leaf needs to be replaced by a node 4
                            current_node = new _Node_4(existing_leaf, existing_key.chunks[i], i);

                            if (i == 0)
                                replace_root(current_node);
                            else
                                current_node->_parent->update_child_ptr(existing_key.chunks[i - 1], current_node);

                            // if the keys are matching, go down all the way until we find a tiebreaker
                            // insert node4's with one child all the way down until a final node 4 with 2 children
                            for (unsigned j = i; j < key_size; j++) {
                                if (existing_key.chunks[j] == transformed_key.chunks[j]) {
                                    Node_ptr new_child = new _Node_4(existing_leaf, existing_key.chunks[j + 1], j + 1);
                                    current_node->update_child_ptr(existing_key.chunks[j], new_child);
                                    current_node = new_child;
                                } else {
                                    Leaf_ptr new_leaf = new _Leaf(__x, current_node);
                                    current_node->insert(transformed_key.chunks[j], new_leaf);
                                    _M_count++;
                                    return make_pair(iterator(new_leaf), true);
                                }
                            }
                            throw; // unreachable
                        }
                    } else {
                        // traverse down the tree
                        previous_node = current_node;
                        current_node = current_node->find(transformed_key.chunks[i]);
                    }
                } else {
                    // hit empty point, this can only happen if the inserted key
                    // is not a prefix/equal to another key already in the tree
                    // therefore we can just insert a new leaf
                    // previous node might have to be grown before that
                    if (previous_node->size() == previous_node->max_size()) {
                        previous_node = grow(previous_node);

                        if (i - 1 == 0)
                            replace_root(previous_node);
                        else
                            previous_node->_parent->update_child_ptr(transformed_key.chunks[i - 2], previous_node);
                    }

                    Leaf_ptr new_leaf = new _Leaf(__x, previous_node);
                    previous_node->insert(transformed_key.chunks[i - 1], new_leaf);
                    _M_count++;
                    return make_pair(iterator(new_leaf), true);
                }
            }
            throw; // unreachable
        }

        template<typename _InputIterator>
        void insert_unique(_InputIterator __first, _InputIterator __last) {
            for (; __first != __last; ++__first)
                insert_unique(*__first);
        }

        template<typename _InputIterator>
        void assign_unique(_InputIterator __first, _InputIterator __last) {
            clear();
            for (; __first != __last; ++__first)
                insert_unique(*__first);
        }

        template<typename... _Args>
        pair<iterator, bool> emplace_unique(_Args &&... __args) {
            Leaf_ptr leaf;
            try {
                // @TODO forward directly into constructor
                leaf = new _Leaf(value_type(std::forward<_Args>(__args)...));
            } catch (...) {
                delete leaf;
                // leaf->~_Leaf();
                throw;
            }

            try {
                Key key = {_M_key_transform(_KeyOfValue()(leaf->_value))};
                pair<Node_ptr, int> __res = get_insert_unique_pos(key);
                if (__res.second >= 0)
                    return pair<iterator, int>(insert_leaf(__res.first, __res.second, leaf, key), true);

                delete leaf;
                return pair<iterator, int>(iterator(__res.first), false);
            } catch (...) {
                delete leaf;
                throw;
            }
        }

    private:
        /**
         * Helper function for emplace. It returns the node where the leaf will
         * be inserted and the leaf's depth, if the key is not already in the tree.
         * If the key exists already, it returns a nullptr and a depth of -1.
         */
        pair<Node_ptr, int32_t> get_insert_unique_pos(const Key &__k) {
            const auto key_size = sizeof(Key);

            // Empty Tree
            if (_M_root == nullptr)
                return pair<Node_ptr, bool>(_M_dummy_node, 0);

            Node_ptr previous_node = _M_dummy_node;
            Node_ptr current_node = _M_root;

            for (int32_t i = 0; i < key_size + 1; i++) {
                if (current_node != nullptr) {
                    if (current_node->is_leaf()) {
                        Leaf_ptr existing_leaf = reinterpret_cast<Leaf_ptr>(current_node);
                        Key existing_key = {_M_key_transform(_KeyOfValue()(existing_leaf->_value))};
                        if (__k.value == existing_key.value) {
                            return pair<Node_ptr, int>(existing_leaf, -1);
                        } else {
                            // otherwise, the leaf needs to be replaced by a node 4
                            current_node = new _Node_4(existing_leaf, existing_key.chunks[i], i);

                            if (i == 0)
                                replace_root(current_node);
                            else
                                current_node->_parent->update_child_ptr(existing_key.chunks[i - 1], current_node);

                            // if the keys are matching, go down all the way until we find a tiebreaker
                            // insert node4's with one child all the way down until a final node 4 with 2 children
                            for (int32_t j = i; j < key_size + 1; j++) {
                                if (existing_key.chunks[j] == __k.chunks[j]) {
                                    Node_ptr new_child = new _Node_4(existing_leaf, existing_key.chunks[j + 1], j + 1);
                                    current_node->update_child_ptr(existing_key.chunks[j], new_child);
                                    current_node = new_child;
                                } else {
                                    return make_pair(current_node, j + 1);
                                }
                            }
                            throw; // unreachable
                        }
                    } else {
                        // traverse down the tree
                        previous_node = current_node;
                        current_node = (current_node)->find(__k.chunks[i]);
                    }
                } else {
                    // hit empty point, this can only happen if the inserted key
                    // is not a prefix/equal to another key already in the tree
                    // therefore we can just insert a new leaf
                    // previous node might have to be grown before that
                    if (previous_node->size() == previous_node->max_size()) {
                        previous_node = grow(previous_node);

                        if (i - 1 == 0)
                            replace_root(previous_node);
                        else
                            previous_node->_parent->update_child_ptr(__k.chunks[i - 2], previous_node);
                    }

                    return make_pair(previous_node, i);
                }
            }
            throw; // unreachable
        }

        /**
         * Helper function for emplace. Inserts the leaf, given a parent and the depth.
         */
        iterator insert_leaf(Node_ptr parent, int32_t depth, Leaf_ptr leaf, const Key &key) {
            leaf->_parent = parent;

            if (depth > 0) {
                parent->insert(key.chunks[depth - 1], leaf);
            } else {
                _M_root = leaf;
                _M_dummy_node->_root = _M_root;
            }
            _M_count++;
            return iterator(leaf);
        };

        inline void replace_root(Node_ptr current_node) {
            _M_root = current_node;
            _M_dummy_node->_root = _M_root;
        }

    public:

        size_type erase_unique(const key_type &__k) {
            // Empty Tree
            if (_M_root == nullptr)
                return 0;

            Key transformed_key = {_M_key_transform(__k)};
            const auto key_size = sizeof(Key);

            if (_M_root->is_leaf()) {
                Key existing_key = {_M_key_transform(_KeyOfValue()(static_cast<Leaf_ptr>(_M_root)->_value))};
                if (transformed_key.value == existing_key.value) {
                    delete _M_root;
                    _M_root = nullptr;
                    _M_count--;
                    return 1;
                }
                return 0;
            }

            Node_ptr current_node = _M_root;

            for (int32_t depth = 0; depth < key_size + 1; depth++) {
                Node_ptr child = current_node->find(transformed_key.chunks[depth]);

                if (child == nullptr)
                    return 0;

                if (child->is_leaf()) {
                    Leaf_ptr existing_leaf = static_cast<Leaf_ptr>(child);
                    Key existing_key = {_M_key_transform(_KeyOfValue()(existing_leaf->_value))};
                    if (transformed_key.value == existing_key.value) {
                        // Delete the leaf
                        current_node->erase(transformed_key.chunks[depth]);
                        _M_count--;

                        fix_after_erase(current_node, depth, transformed_key);

                        return 1;
                    } else {
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
            Key transformed_key = {_M_key_transform(_KeyOfValue()(*__it))};
            Leaf_ptr leaf = static_cast<Leaf_ptr >(__it.node);

            // Edge case, leaf is the root/last element in container
            if (leaf == _M_root) {
                delete leaf;
                _M_root = nullptr;
                _M_count--;
                return __result;
            }

            Node_ptr inner_node = leaf->_parent;
            int32_t depth = static_cast<Inner_Node_ptr>(inner_node)->_depth;
            inner_node->erase(transformed_key.chunks[depth]);
            _M_count--;

            fix_after_erase(inner_node, depth, transformed_key);

            return __result;
        }

    private:
        void fix_after_erase(Node_ptr current_node, int32_t depth, const Key &transformed_key) {
            // Shrink node if necessary & delete obsolete nodes
            Node_ptr shrunk_node = shrink_after_erase(current_node, depth, transformed_key);

            // Node wasn't changed
            if (shrunk_node == current_node)
                return;

            // Update child pointer in parent of inner_node
            current_node = shrunk_node;
            Node_ptr parent = current_node->_parent;

            if (parent->get_type() != _dummy_node_t)
                parent->update_child_ptr(
                        transformed_key.chunks[static_cast<Inner_Node_ptr>(parent)->_depth],
                        current_node);
            else
                replace_root(current_node);
        }

        /**
         * @brief Attempts to shrink the node and recursively remove one-way parents above it.
         * @param node Node that might be shrunk and whose one-way parents are dropped.
         * @param depth
         * @param key
         * @return A pointer to the unchanged or shrunk node.
         */
        Node_ptr shrink_after_erase(Node_ptr node, int32_t depth, const Key &key) {
            // Parent of deleted leaf now underfull?
            int32_t j = 1;
            while (node->size() < node->min_size()) {
                pair<Node_ptr, bool> p = shrink(node);

                // Cannot shrink node 4 to leaf, because child is not a leaf
                if (!p.second)
                    return node;

                node = p.first;

                // As long as the node above the leaf is a one-way node, compress path
                while (node->is_leaf() && node->_parent->size() <= 1 && depth - j >= -1) {
                    Node_ptr parent = node->_parent;
                    if (parent->get_type() == node_type::_dummy_node_t) {
                        node->_parent = _M_dummy_node;
                        return node;
                    }

                    Node_ptr grandparent = parent->_parent;
                    if (grandparent->get_type() == node_type::_dummy_node_t) {
                        node->_parent = _M_dummy_node;
                        delete parent;
                        return node;
                    }
                    grandparent->update_child_ptr(key.chunks[depth - j - 1], node);
                    node->_parent = grandparent;
                    delete parent;
                    j++;
                }
            }
            return node;
        }

    public:
        void swap(adaptive_radix_tree &__x) {
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

            Node_ptr current_node = _M_root;
            for (unsigned i = 0; i < key_size + 1; i++) {
                if (current_node == nullptr)
                    return end();

                if (current_node->is_leaf()) {
                    Leaf_ptr leaf = static_cast<Leaf_ptr>(current_node);
                    Key existing_key = {_M_key_transform(_KeyOfValue()(leaf->_value))};
                    if (transformed_key.value == existing_key.value)
                        return iterator(current_node);
                    return end();
                }

                current_node = current_node->find(transformed_key.chunks[i]);
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
                    Const_Leaf_ptr leaf = static_cast<Const_Leaf_ptr>(current_node);
                    Key existing_key = {_M_key_transform(_KeyOfValue()(leaf->_value))};
                    if (transformed_key.value == existing_key.value)
                        return const_iterator(current_node);
                    return end();
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

            Node_ptr previous_node = nullptr;
            Node_ptr current_node = _M_root;
            for (unsigned i = 0; i < key_size + 1; i++) {
                if (current_node == nullptr) {
                    auto successor = previous_node->successor(transformed_key);
                    return iterator(successor);
                }

                if (current_node->is_leaf()) {
                    Leaf_ptr leaf = static_cast<Leaf_ptr>(current_node);
                    Key existing_key = {_M_key_transform(_KeyOfValue()(leaf->_value))};

                    for (unsigned j = i; j < key_size; j++) {
                        if (transformed_key.chunks[j] > existing_key.chunks[j]) {
                            auto successor = previous_node->successor(transformed_key);
                            return iterator(successor);
                        }
                    }
                    return iterator(current_node);
                }

                previous_node = current_node;
                current_node = current_node->find(transformed_key.chunks[i]);
            }
            throw; // unreachable
        }

        const_iterator lower_bound(const key_type &__k) const {
            Key transformed_key = {_M_key_transform(__k)};
            const auto key_size = sizeof(__k);

            if (_M_root == nullptr)
                return end();

            Node_ptr previous_node = nullptr;
            Node_ptr current_node = _M_root;
            for (unsigned i = 0; i < key_size + 1; i++) {
                if (current_node == nullptr) {
                    auto successor = previous_node->successor(transformed_key);
                    return const_iterator(successor);
                }

                if (current_node->is_leaf()) {
                    Leaf_ptr leaf = static_cast<Leaf_ptr>(current_node);
                    Key existing_key = {_M_key_transform(_KeyOfValue()(leaf->_value))};

                    for (unsigned j = i; j < key_size; j++) {
                        if (transformed_key.chunks[j] > existing_key.chunks[j]) {
                            auto successor = previous_node->successor(transformed_key);
                            return const_iterator(successor);
                        }
                    }
                    return const_iterator(current_node);
                }

                previous_node = current_node;
                current_node = current_node->find(transformed_key.chunks[i]);
            }
            throw; // unreachable
        }

        iterator upper_bound(const key_type &__k) {
            Key transformed_key{_M_key_transform(__k)};
            const auto key_size = sizeof(__k);

            if (_M_root == nullptr)
                return end();

            Node_ptr previous_node = nullptr;
            Node_ptr current_node = _M_root;
            for (unsigned i = 0; i < key_size + 1; i++) {
                if (current_node == nullptr) {
                    auto successor = previous_node->successor(transformed_key);
                    return iterator(successor);
                }

                if (current_node->is_leaf()) {
                    Leaf_ptr leaf = static_cast<Leaf_ptr>(current_node);
                    Key existing_key = {_M_key_transform(_KeyOfValue()(leaf->_value))};

                    for (unsigned j = i; j < key_size; j++)
                        if (transformed_key.chunks[j] < existing_key.chunks[j])
                            return iterator(current_node);

                    auto successor = previous_node->successor(transformed_key);
                    return iterator(successor);
                }

                previous_node = current_node;
                current_node = current_node->find(transformed_key.chunks[i]);
            }
            throw; // unreachable
        }

        const_iterator upper_bound(const key_type &__k) const {
            Key transformed_key = {_M_key_transform(__k)};
            const auto key_size = sizeof(__k);

            if (_M_root == nullptr)
                return end();

            Node_ptr previous_node = nullptr;
            Node_ptr current_node = _M_root;
            for (unsigned i = 0; i < key_size + 1; i++) {
                if (current_node == nullptr) {
                    auto successor = previous_node->successor(transformed_key);
                    return const_iterator(successor);
                }

                if (current_node->is_leaf()) {
                    Leaf_ptr leaf = static_cast<Leaf_ptr>(current_node);
                    Key existing_key = {_M_key_transform(_KeyOfValue()(leaf->_value))};

                    for (unsigned j = i; j < key_size; j++)
                        if (transformed_key.chunks[j] < existing_key.chunks[j])
                            return const_iterator(current_node);

                    auto successor = previous_node->successor(transformed_key);
                    return const_iterator(successor);
                }

                previous_node = current_node;
                current_node = current_node->find(transformed_key.chunks[i]);
            }
            throw; // unreachable
        }

        pair<iterator, iterator> equal_range(const key_type &__k) {
            auto lower = lower_bound(__k);
            auto upper = upper_bound(__k);
            return pair<iterator, iterator>(lower, upper);
        };

        pair<const_iterator, const_iterator> equal_range(const key_type &__k) const {
            auto lower = lower_bound(__k);
            auto upper = upper_bound(__k);
            return pair<const_iterator, const_iterator>(lower, upper);
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

        ~adaptive_radix_tree() {
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
                default:
                    throw;
            }
            throw; // unreachable
        }

        pair<Node_ptr, bool> shrink(Node_ptr old_node) {
            auto type = old_node->get_type();
            switch (type) {
                case node_type::node_4_t: {
                    _Node_4 *node4 = static_cast<_Node_4 *>(old_node);

                    if (node4->children[0]->is_leaf()) {
                        node4->children[0]->_parent = old_node->_parent;
                        Leaf_ptr child = static_cast<Leaf_ptr >(node4->children[0]);
                        delete node4;
                        return pair<Node_ptr, bool>(child, true);
                    }
                    return pair<Node_ptr, bool>(old_node, false);
                }
                case node_type::node_16_t: {
                    _Node_16 *node16 = static_cast<_Node_16 *>(old_node);
                    Node_ptr node = new _Node_4(node16);
                    delete node16;
                    return pair<Node_ptr, bool>(node, true);
                }
                case node_type::node_48_t: {
                    _Node_48 *node48 = static_cast<_Node_48 *>(old_node);
                    Node_ptr node = new _Node_16(node48);
                    delete node48;
                    return pair<Node_ptr, bool>(node, true);
                }
                case node_type::node_256_t: {
                    _Node_256 *node256 = static_cast<_Node_256 *>(old_node);
                    Node_ptr node = new _Node_48(node256);
                    delete node256;
                    return pair<Node_ptr, bool>(node, true);
                }
                default:
                    throw;
            }
            throw; // unreachable
        }
    };

    //////////////////////////
    // Relational Operators //
    //////////////////////////

    template<typename _Key, typename _Tp, typename _KeyOfValue, typename _Key_transform>
    inline bool
    operator==(const adaptive_radix_tree<_Key, _Tp, _KeyOfValue, _Key_transform> &lhs,
               const adaptive_radix_tree<_Key, _Tp, _KeyOfValue, _Key_transform> &rhs) {
        return lhs.size() == rhs.size()
               && std::equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    // Based on operator==
    template<typename _Key, typename _Tp, typename _KeyOfValue, typename _Key_transform>
    inline bool
    operator!=(const adaptive_radix_tree<_Key, _Tp, _KeyOfValue, _Key_transform> &lhs,
               const adaptive_radix_tree<_Key, _Tp, _KeyOfValue, _Key_transform> &rhs) {
        return !(lhs == rhs);
    }

    template<typename _Key, typename _Tp, typename _KeyOfValue, typename _Key_transform>
    inline bool
    operator<(const adaptive_radix_tree<_Key, _Tp, _KeyOfValue, _Key_transform> &__x,
              const adaptive_radix_tree<_Key, _Tp, _KeyOfValue, _Key_transform> &__y) {
        return std::lexicographical_compare(__x.begin(), __x.end(),
                                            __y.begin(), __y.end());
    }

    // Based on operator<
    template<typename _Key, typename _Tp, typename _KeyOfValue, typename _Key_transform>
    inline bool
    operator>(const adaptive_radix_tree<_Key, _Tp, _KeyOfValue, _Key_transform> &__x,
              const adaptive_radix_tree<_Key, _Tp, _KeyOfValue, _Key_transform> &__y) {
        return __y < __x;
    }

    // Based on operator<
    template<typename _Key, typename _Tp, typename _KeyOfValue, typename _Key_transform>
    inline bool
    operator<=(const adaptive_radix_tree<_Key, _Tp, _KeyOfValue, _Key_transform> &__x,
               const adaptive_radix_tree<_Key, _Tp, _KeyOfValue, _Key_transform> &__y) {
        return !(__y < __x);
    }

    // Based on operator<
    template<typename _Key, typename _Tp, typename _KeyOfValue, typename _Key_transform>
    inline bool
    operator>=(const adaptive_radix_tree<_Key, _Tp, _KeyOfValue, _Key_transform> &__x,
               const adaptive_radix_tree<_Key, _Tp, _KeyOfValue, _Key_transform> &__y) {
        return !(__x < __y);
    }

}


#endif //ART_ADAPT_RADIX_TREE_H
