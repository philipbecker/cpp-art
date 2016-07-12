#ifndef ART_ADAPT_RADIX_TREE_H
#define ART_ADAPT_RADIX_TREE_H

#include <stddef.h>
#include <assert.h>
#include <bits/unique_ptr.h>
#include <utility>
#include <bitset>
#include <iostream>
#include <stack>

namespace art
{
    template<class T>
    struct key_transformer {

        T operator()(T &v) {
            return key_transform(v);
        }

        T operator()(const T &v) {
            return key_transform(v);
        }

        uint16_t byte_swap(uint16_t val) {
            return (val << 8) | (val >> 8);
        }

        int16_t byte_swap(int16_t val) {
            return (val << 8) | ((val >> 8) & 0xFF);
        }

        uint32_t byte_swap(uint32_t val) {
            val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
            return (val << 16) | (val >> 16);
        }

        int32_t byte_swap(int32_t val) {
            val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
            return (val << 16) | ((val >> 16) & 0xFFFF);
        }

        int64_t byte_swap(int64_t val) {
            val = ((val << 8) & 0xFF00FF00FF00FF00ULL) | ((val >> 8) & 0x00FF00FF00FF00FFULL);
            val = ((val << 16) & 0xFFFF0000FFFF0000ULL) | ((val >> 16) & 0x0000FFFF0000FFFFULL);
            return (val << 32) | ((val >> 32) & 0xFFFFFFFFULL);
        }

        uint64_t byte_swap(uint64_t val) {
            val = ((val << 8) & 0xFF00FF00FF00FF00ULL) | ((val >> 8) & 0x00FF00FF00FF00FFULL);
            val = ((val << 16) & 0xFFFF0000FFFF0000ULL) | ((val >> 16) & 0x0000FFFF0000FFFFULL);
            return (val << 32) | (val >> 32);
        }

        inline bool is_big_endian() {
            int num = 1;
            return !(*(char *) &num == 1);
        }

        //       typename std::enable_if<std::is_integral<T>::value, T>::type

        T key_transform(T n) {
            if (std::is_signed<T>::value)
                n ^= 1 << ((sizeof(T) * 8) - 1);
            if (is_big_endian())
                return n;
            return byte_swap(n);
        }
    };

    static const uint8_t EMPTY_MARKER = 48;

    template<typename _Key, typename _Tp>
    class adapt_radix_tree {
    private:
        size_t _count;
        key_transformer<_Key> transformer;

        typedef _Key key_type;
        typedef _Tp mapped_type;
        typedef std::pair<const _Key, _Tp> value_type;


        enum node_type : uint8_t {
            _leaf_t = 0, node_4_t = 1, node_16_t = 2, node_48_t = 3, node_256_t = 4
        };

    public:
        union Key {
            const key_type value;
            const uint8_t chunks[sizeof(key_type)];
        };

        class _node;

        typedef std::stack<std::pair<_node *, unsigned>> parent_iter_stack;

        class _node {
        protected:
            uint16_t _count;

        public:
            virtual _node *insert(const Key &key, unsigned depth) = 0;

            virtual void insert(const uint8_t &key_byte, _node *node) = 0;

            virtual _node **find(const uint8_t &key_byte) = 0;

            virtual _node *minimum() = 0;

            virtual _node *minimum(parent_iter_stack &parents) = 0;

            virtual _node *maximum() = 0;

            virtual _node *maximum(parent_iter_stack &parents) = 0;

            virtual void traverse(unsigned depth) = 0;

            virtual bool is_leaf() {
                return false;
            }

            uint16_t size() const {
                return _count;
            }

            virtual uint16_t max_size() const = 0;

            virtual node_type get_type() const = 0;

            virtual
            std::pair<bool, _node *>
                    next(unsigned pos, parent_iter_stack &parents) = 0;
        };

        class _leaf : public _node {
        public:
            Key key;
            mapped_type value;

            _leaf(Key key) : key(key) {
                this->_count = 1;
            }

            _leaf(Key key, mapped_type value) : key(key), value(value) {
                this->_count = 1;
            }

            virtual _node *insert(const Key &key, unsigned depth) override {
                return this;
            }

            virtual void insert(const uint8_t &key_byte, _node *node) override {

            }

            virtual void traverse(unsigned depth) override {
                std::cout << std::string(depth + 1, '-') << " leaf " << this << ": " << value << std::endl;
            }

            virtual _node **find(const uint8_t &key_byte) override {
                return nullptr;
            }

            virtual _node *minimum() override {
                return this;
            }

            virtual _node *minimum(parent_iter_stack &parents) override {
                return this;
            }

            virtual _node *maximum() override {
                return this;
            }

            virtual _node *maximum(parent_iter_stack &parents) override {
                return this;
            }

            virtual std::pair<bool, _node *> next(unsigned pos,
                                                  parent_iter_stack &parents) override {
                return std::pair<bool, _node *>(true, this);
            }

            bool contains(const Key &key, unsigned depth) {
                return this->key.value == key.value;
            }

            bool is_leaf() override {
                return true;
            }

            uint16_t max_size() const override {
                return 1;
            }

            virtual node_type get_type() const override {
                return node_type::_leaf_t;
            }

            Key &get_key() {
                return key;
            }
        };

        // Forward declarations
        class node_16;

        class node_48;

        class node_256;

        class node_4 : public _node {
        public:
            std::array<uint8_t, 4> keys{};
            std::array<_node *, 4> children{};

            node_4(_leaf *leaf, unsigned depth) {
                Key key = leaf->get_key();
                keys[0] = key.chunks[depth];
                children[0] = leaf;

                this->_count = 1;
            }

            node_4(node_16 *node) {
                assert(node->size() == 4);

                std::copy(node->keys.begin(), node->keys.begin() + 4, keys.begin());
                std::copy(node->children.begin(), node->children.begin() + 4, children.begin());
                _count = 4;
                delete node;
            }

            virtual uint16_t max_size() const override {
                return 4;
            }

            virtual _node *insert(const Key &key, unsigned depth) override {
                return nullptr;
            }

            virtual void insert(const uint8_t &key_byte, _node *node) override {
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

            virtual _node **find(const uint8_t &key_byte) override {
                unsigned pos = 0;
                for (; pos < this->_count && keys[pos] < key_byte; pos++);

                if (keys[pos] == key_byte)
                    return &children[pos];
                return nullptr;
            }

            virtual _node *minimum() override {
                return children[0]->minimum();
            }

            virtual _node *minimum(parent_iter_stack &parents) override {
                parents.push(std::make_pair(this, 0));
                return children[0]->minimum(parents);
            }

            virtual _node *maximum() override {
                return children[this->_count - 1]->maximum();
            }

            virtual _node *maximum(parent_iter_stack &parents) override {
                auto max = this->_count - 1;
                parents.push(std::make_pair(this, max));
                return children[max]->maximum(parents);
            }

            virtual void traverse(unsigned depth) override {
                for (size_t i = 0; i < this->_count; i++)
                    children[i]->traverse(depth + 1);
            }

            virtual std::pair<bool, _node *> next(unsigned pos,
                                                  parent_iter_stack &parents) override {
                if (pos < this->_count) {
                    parents.emplace(this, pos);
                    return children[pos]->next(0, parents);
                }
                return std::pair<bool, _node *>(false, nullptr);
            }

            virtual node_type get_type() const override {
                return node_type::node_4_t;
            }

        };

        class node_16 : public _node {
        public:
            std::array<uint8_t, 16> keys{};
            std::array<_node *, 16> children{};

            node_16(node_4 *node) {
                assert(node->size() == 4);

                std::copy(node->keys.begin(), node->keys.end(), keys.begin());
                std::copy(node->children.begin(), node->children.end(), children.begin());
                this->_count = 4;
                delete node;
            }

            node_16(node_48 *node) {
                assert(node->size() == 16);

                uint8_t pos = 0;
                for (uint16_t i = 0; i < 256; i++) {
                    if (node->child_index[i] != EMPTY_MARKER) {
                        keys[pos] = i;
                        children[pos] = node->children[node->child_index[i]];
                        pos++;
                    }
                }
                this->_count = 16;

                delete node;
            }

            virtual _node *insert(const Key &key, unsigned depth) override {
                return nullptr;
            }

            virtual void insert(const uint8_t &key_byte, _node *node) override {
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

            virtual _node **find(const uint8_t &key_byte) override {
                unsigned pos = 0;
                for (; pos < this->_count && keys[pos] < key_byte; pos++);

                if (keys[pos] == key_byte)
                    return &children[pos];
                return nullptr;
            }

            virtual void traverse(unsigned depth) override {
                for (uint8_t i = 0; i < this->_count; i++)
                    children[i]->traverse(depth + 1);
            }

            virtual _node *minimum() override {
                return children[0]->minimum();
            }

            virtual _node *minimum(parent_iter_stack &parents) override {
                parents.push(std::make_pair(this, 0));
                return children[0]->minimum(parents);
            }

            virtual _node *maximum() override {
                return children[this->_count - 1]->maximum();
            }

            virtual _node *maximum(parent_iter_stack &parents) override {
                auto max = this->_count - 1;
                parents.push(std::make_pair(this, max));
                return children[max]->maximum(parents);
            }

            virtual std::pair<bool, _node *> next(unsigned pos,
                                                  parent_iter_stack &parents) override {
                if (pos < this->_count) {
                    parents.emplace(this, pos);
                    return children[pos]->next(0, parents);
                }
                return std::pair<bool, _node *>(false, nullptr);
            }

            virtual uint16_t max_size() const override {
                return 16;
            }

            virtual node_type get_type() const override {
                return node_type::node_16_t;
            }
        };


        class node_48 : public _node {
        public:
            std::array<uint8_t, 256> child_index;
            std::array<_node *, 48> children{};

            node_48() {
                _count = 0;
                std::fill(child_index.begin(), child_index.end(), EMPTY_MARKER);
            }

            node_48(node_16 *node) {
                assert(node->size() == 16);

                std::fill(child_index.begin(), child_index.end(), EMPTY_MARKER);

                for (uint8_t i = 0; i < 16; i++) {
                    child_index[node->keys[i]] = i;
                    children[i] = node->children[i];
                }
                this->_count = 16;

                delete node;
            }

            node_48(node_256 *node) {
                assert(node->size() == 16);

                std::fill(child_index.begin(), child_index.end(), EMPTY_MARKER);

                uint8_t pos = 0;
                for (uint16_t i = 0; i < 256; i++) {
                    if (node->children[i] != nullptr) {
                        child_index[i] = pos;
                        children[pos] = node->children[i];
                        pos++;
                    }
                }
                this->_count = 16;

                delete node;
            }

            virtual _node *insert(const Key &key, unsigned depth) override {
                return nullptr;
            }

            virtual void insert(const uint8_t &key_byte, _node *node) override {
                auto pos = this->_count;
                child_index[key_byte] = pos;
                children[pos] = node;
                this->_count++;
            }

            virtual _node **find(const uint8_t &key_byte) override {
                if (child_index[key_byte] != EMPTY_MARKER)
                    return &children[child_index[key_byte]];
                return nullptr;
            }

            virtual _node *minimum() override {
                unsigned pos = 0;
                while (child_index[pos] == EMPTY_MARKER)
                    pos++;
                return children[child_index[pos]]->minimum();
            }

            virtual _node *minimum(parent_iter_stack &parents) override {
                unsigned pos = 0;
                while (child_index[pos] == EMPTY_MARKER)
                    pos++;
                parents.push(std::make_pair(this, pos));
                return children[child_index[pos]]->minimum(parents);
            }

            virtual _node *maximum() override {
                auto pos = max_size() - 1;
                while (child_index[pos] == EMPTY_MARKER)
                    pos--;
                return children[child_index[pos]]->maximum();
            }

            virtual _node *maximum(parent_iter_stack &parents) override {
                auto pos = max_size() - 1;
                while (child_index[pos] == EMPTY_MARKER)
                    pos--;
                parents.push(std::make_pair(this, pos));
                return children[child_index[pos]]->maximum(parents);
            }

            virtual void traverse(unsigned depth) override {
                for (uint8_t i = 0; i < this->_count; i++)
                    children[i]->traverse(depth + 1);
            }

            virtual std::pair<bool, _node *> next(unsigned pos,
                                                  parent_iter_stack &parents) override {
                if (pos < this->_count) {
                    parents.emplace(this, pos);
                    return children[child_index[pos]]->next(0, parents);
                }
                return std::pair<bool, _node *>(false, nullptr);
            }

            virtual uint16_t max_size() const override {
                return 48;
            }

            virtual node_type get_type() const override {
                return node_type::node_48_t;
            }
        };

        class node_256 : public _node {
        public:
            std::array<_node *, 256> children{};

            node_256() { }

            node_256(_leaf *pLeaf, unsigned int depth) {
                Key key = pLeaf->get_key();
                children[key.chunks[depth]] = pLeaf;

                this->_count = 1;
            }

            node_256(node_48 *node) {
                assert(node->size() == 48);

                for (uint16_t i = 0; i < 256; i++)
                    if (node->child_index[i] != EMPTY_MARKER)
                        children[i] = node->children[node->child_index[i]];
                this->_count = 48;

                delete node;
            }

            virtual _node *insert(const Key &key, unsigned depth) override {
                children[key.chunks[depth]] = new _leaf(key);
                this->_count++;
                return children[key.chunks[depth]];
            }

            virtual void insert(const uint8_t &key_byte, _node *node) override {
                children[key_byte] = node;
                this->_count++;
            }

            virtual _node **find(const uint8_t &key_byte) override {
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

            virtual _node *minimum() override {
                unsigned pos = 0;
                while (children[pos] == nullptr)
                    pos++;
                return children[pos]->minimum();
            }

            virtual _node *minimum(parent_iter_stack &parents) override {
                unsigned pos = 0;
                while (children[pos] == nullptr)
                    pos++;
                parents.push(std::make_pair(this, pos));
                return children[pos]->minimum(parents);
            }

            virtual _node *maximum() override {
                auto pos = max_size() - 1;
                while (children[pos] == nullptr)
                    pos--;
                return children[pos]->maximum();
            }

            virtual _node *maximum(parent_iter_stack &parents) override {
                auto pos = max_size() - 1;
                while (children[pos] == nullptr)
                    pos--;
                parents.push(std::make_pair(this, pos));
                return children[pos]->maximum(parents);
            }

            virtual std::pair<bool, _node *> next(unsigned pos, parent_iter_stack &parents) override {
                for (; pos < 256; pos++) {
                    if (children[pos] != nullptr) {
                        parents.emplace(this, pos);
                        return children[pos]->next(0, parents);
                    }
                }
                return std::pair<bool, _node *>(false, nullptr);
            }

            virtual uint16_t max_size() const override {
                return 256;
            }

            virtual node_type get_type() const override {
                return node_type::node_256_t;
            }

        };


    public:
        _node *_root;
        _node *_empty_tree = new _leaf(Key{0}, 0);

        adapt_radix_tree() : _count(0) {
            this->_root = nullptr;
        }

        std::pair<_node *, bool> insert(const value_type &x) {
            Key debugKey = {x.first};
            Key key = {transformer(x.first)};

            _leaf *new_leaf = new _leaf(key, x.second);

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
                    // is not a prefix/equal to another key already in the tree
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
            Key key = {transformer(x)};

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

        _node *minimum() {
            if (_root != nullptr)
                return _root->minimum();
            return nullptr;
        }

        _node *minimum(parent_iter_stack &parents) {
            if (_root != nullptr)
                return _root->minimum(parents);
            return nullptr;
        }

        _node *maximum() {
            if (_root != nullptr)
                return _root->maximum();
            return nullptr;
        }

        _node *maximum(parent_iter_stack &parents) {
            if (_root != nullptr)
                return _root->maximum(parents);
            return nullptr;
        }

        void traverse() {
            _root->traverse(0);
        }

        size_t size() const {
            return _count;
        }

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


        class adapt_radix_tree_iterator {
            typedef _Tp value_type;
            typedef _Tp &reference;
            typedef _Tp *pointer;

            typedef adapt_radix_tree_iterator _Self;
            typedef _node *_Base_ptr;
            typedef _leaf *_Link_type;

        private:
            _node *node;
            std::stack<std::pair<_node *, unsigned> > parents;

        public:
            explicit adapt_radix_tree_iterator(_node *node)
                    : node(node) {
                //std::cout << "is leaf ? : " << node->is_leaf() << std::endl;
            }

            explicit adapt_radix_tree_iterator(_node *node, parent_iter_stack &parents)
                    : node(node), parents(parents) {

            }

            // preincrement
            _Self &operator++() {
                _node *current_node;
                uint16_t current_pos;
                while (!parents.empty()) {
                    std::tie(current_node, current_pos) = parents.top();
                    parents.pop();
                    auto next = current_node->next(current_pos + 1, parents);
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

            bool operator==(_Self other) const {
                return node == other.node;
            }

            bool operator!=(_Self other) const {
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
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        iterator begin() {
            parent_iter_stack parents;
            auto min = minimum(parents);
            return iterator(min, parents);
        }

        iterator end() {
            return iterator(nullptr);
        }

        iterator rbegin() {
            return iterator(nullptr);
        }

        iterator rend() {
            parent_iter_stack parents;
            auto max = maximum(parents);
            return iterator(max, parents);
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
                    node_48 *node48 = static_cast<node_48 *>(old_node);
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
