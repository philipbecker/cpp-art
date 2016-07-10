#ifndef REFERENCE_ART_MAP_H
#define REFERENCE_ART_MAP_H

#include "adapt_radix_tree.h"

namespace art
{
    template<typename _Key, typename _Tp>
    class map {

    public:
        typedef _Key key_type;
        typedef _Tp mapped_type;
        typedef std::pair<const _Key, _Tp> value_type;

        typedef adapt_radix_tree<key_type, value_type> _Rep_type;
        _Rep_type _M_t;

        typedef typename _Rep_type::iterator iterator;
        typedef typename _Rep_type::const_iterator const_iterator;
        typedef typename _Rep_type::size_type size_type;
        typedef typename _Rep_type::difference_type difference_type;
        typedef typename _Rep_type::reverse_iterator reverse_iterator;
        typedef typename _Rep_type::const_reverse_iterator const_reverse_iterator;

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
        map(map &&__x) : _M_t(std::move(__x._M_t)) { }

        /**
         * Returns true if the map is empty.  (Thus begin() would equal
         *  end().)
        */
        bool empty() const { return _M_t.empty(); }

        /**
         * Returns the size of the map.
         */
        size_type size() const { return _M_t.size(); }

        mapped_type &operator[](const key_type &__k) {
            // concept requirements
            __glibcxx_function_requires(_DefaultConstructibleConcept < mapped_type >)

            iterator __i = lower_bound(__k);
            // __i->first is greater than or equivalent to __k.
            if (__i == end() || key_comp()(__k, (*__i).first))
                __i = _M_t._M_emplace_hint_unique(__i, std::piecewise_construct,
                                                  std::tuple<const key_type &>(__k),
                                                  std::tuple<>());

            return (*__i).second;
        }

        std::pair<iterator, bool>
        insert(const value_type &__x) {
            return _M_t.insert(__x);
        }
    };
}

#endif //REFERENCE_ART_MAP_H
