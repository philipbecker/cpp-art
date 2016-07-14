#ifndef REFERENCE_ART_MAP_H
#define REFERENCE_ART_MAP_H

#include "Adaptive_radix_tree.h"

namespace art
{
    template<typename _Key, typename _Tp>
    class map {

    public:
        typedef _Key key_type;
        typedef _Tp mapped_type;
        typedef std::pair<const _Key, _Tp> value_type;

        typedef Adaptive_radix_tree<key_type, mapped_type> _Rep_type;
        _Rep_type _M_t;

        typedef typename _Rep_type::iterator iterator;
//        typedef typename _Rep_type::const_iterator const_iterator;
        typedef typename _Rep_type::size_type size_type;
        typedef typename _Rep_type::difference_type difference_type;
//        typedef typename _Rep_type::reverse_iterator reverse_iterator;
//        typedef typename _Rep_type::const_reverse_iterator const_reverse_iterator;

        /**
         * @brief  Default constructor creates no elements.
         */
        map() : _M_t() { }

        /**
         * @brief  Map copy constructor.
         */
        map(const map &__x) : _M_t(__x._M_t) { }

        /**
         * @brief  Map move constructor.
         */
        //map(map &&__x) : _M_t(std::move(__x._M_t)) { }

        /**
         * Returns true if the map is empty.  (Thus begin() would equal
         *  end().)
        */
        bool empty() const { return _M_t.empty(); }

        /**
         * Returns the size of the map.
         */
        size_type size() const { return _M_t.size(); }

        void
        insert(const value_type &__x) {
            _M_t.insert(__x);
        }

        /**

        mapped_type &operator[](const key_type &__k) {

        }

        std::pair<iterator, bool>
        insert(const value_type &__x) {
            return _M_t.insert(__x);
        }

        iterator
        find(const key_type& __x)
        { return _M_t.find(__x); }

        size_type
        count(const key_type& __x) const
        { return _M_t.find(__x) == _M_t.end() ? 0 : 1; }
         */
    };
}

#endif //REFERENCE_ART_MAP_H
