#ifndef REFERENCE_ART_MAP_H
#define REFERENCE_ART_MAP_H

#include "Adaptive_radix_tree.h"

namespace art
{
    template<typename _Key, typename _Tp,
            typename _Key_transform = key_transform<_Key> >
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
        typedef typename _Rep_type::reverse_iterator reverse_iterator;
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
         *  @brief  Builds a %map from a range.
         *  @param  __first  An input iterator.
         *  @param  __last  An input iterator.
         *
         */
        template<typename _InputIterator>
        map(_InputIterator __first, _InputIterator __last)
                : _M_t() {
            throw;
            // _M_t._M_insert_unique(__first, __last);
        }

        //////////////
        // Capacity //
        //////////////

        /**
         * Returns the size of the map.
         */
        size_type size() const { return _M_t.size(); }

        /**
         * Returns true if the map is empty.  (Thus begin() would equal
         *  end().)
        */
        bool empty() const { return _M_t.empty(); }

        void insert(const value_type &__x) {
            _M_t.insert(__x);
        }

        ///////////////
        // Iterators //
        ///////////////

        iterator begin() {
            return _M_t.begin();
        }

        iterator end() {
            return _M_t.end();
        }

        reverse_iterator rbegin() {
            return _M_t.rbegin();
        }

        reverse_iterator rend() {
            return _M_t.rend();
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

    //////////////////////////
    // Relational Operators //
    //////////////////////////

    /**
     *  @brief  Map equality comparison.
     *  @param  __x  A %map.
     *  @param  __y  A %map of the same type as @a x.
     *  @return  True iff the size and elements of the maps are equal.
     *
     *  This is an equivalence relation.  It is linear in the size of the
     *  maps.  Maps are considered equivalent if their sizes are equal,
     *  and if corresponding elements compare equal.
    */
    template<typename _Key, typename _Tp, typename _Key_transform>
    inline bool
    operator==(const map<_Key, _Tp, _Key_transform> &__x,
               const map<_Key, _Tp, _Key_transform> &__y) {
        return __x._M_t == __y._M_t;
    }

    /**
     *  @brief  Map ordering relation.
     *  @param  __x  A %map.
     *  @param  __y  A %map of the same type as @a x.
     *  @return  True iff @a x is lexicographically less than @a y.
     *
     *  This is a total ordering relation.  It is linear in the size of the
     *  maps.  The elements must be comparable with @c <.
     *
     *  See std::lexicographical_compare() for how the determination is made.
    */
    template<typename _Key, typename _Tp, typename _Key_transform>
    inline bool
    operator<(const map<_Key, _Tp, _Key_transform> &__x,
              const map<_Key, _Tp, _Key_transform> &__y) {
        return __x._M_t < __y._M_t;
    }

    // Based on operator==
    template<typename _Key, typename _Tp, typename _Key_transform>
    inline bool
    operator!=(const map<_Key, _Tp, _Key_transform> &__x,
               const map<_Key, _Tp, _Key_transform> &__y) {
        return !(__x == __y);
    }

    // Based on operator<
    template<typename _Key, typename _Tp, typename _Key_transform>
    inline bool
    operator>(const map<_Key, _Tp, _Key_transform> &__x,
              const map<_Key, _Tp, _Key_transform> &__y) {
        return __y < __x;
    }

    // Based on operator<
    template<typename _Key, typename _Tp, typename _Key_transform>
    inline bool
    operator<=(const map<_Key, _Tp, _Key_transform> &__x,
               const map<_Key, _Tp, _Key_transform> &__y) {
        return !(__y < __x);
    }

    // Based on operator<
    template<typename _Key, typename _Tp, typename _Key_transform>
    inline bool
    operator>=(const map<_Key, _Tp, _Key_transform> &__x,
               const map<_Key, _Tp, _Key_transform> &__y) {
        return !(__x < __y);
    }
}

#endif //REFERENCE_ART_MAP_H
