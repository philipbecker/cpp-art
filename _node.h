#ifndef ART_NODE_H
#define ART_NODE_H

#include <cstdint>
#include <iostream>
#include <bitset>

namespace art
{
    union Key {
        const int value;
        const uint8_t chunks[sizeof(int)];
    };


    enum node_type : uint8_t {
        _leaf_t = 0, node_4_t = 1, node_16_t = 2, node_48_t = 3, node_256_t = 4
    };

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

    const uint8_t EMPTY_MARKER = 48;

    class _node {
    protected:
        uint16_t _count;

    public:
        virtual _node *insert(const Key &key, unsigned depth) = 0;

        virtual void insert(const uint8_t &key_byte, _node *node) = 0;

        virtual _node **find(const uint8_t &key_byte) = 0;

        virtual void traverse(unsigned depth) = 0;

        virtual bool is_leaf() {
            return false;
        }

        uint16_t size() const {
            return _count;
        }

        virtual uint16_t max_size() const = 0;

        virtual node_type get_type() const = 0;
    };
}

#endif //ART_NODE_H
