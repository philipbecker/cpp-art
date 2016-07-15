#ifndef REFERENCE_ART_MAP_H
#define REFERENCE_ART_MAP_H

#include "Adaptive_radix_tree.h"

namespace art
{
    /**
     * @brief A standard container made up of (key,value) pairs, which can be
     * retrieved based on a key, in linear time in size of the key.
     *
     *  @tparam _Key  Type of key objects.
     *  @tparam  _Tp  Type of mapped objects.
     *  @tparam _Key_transform  Key transformation function object type,
     *                          defaults to key_transform<_Key>.
     *  @tparam _Alloc  Allocator type, defaults to
     *                  allocator<pair<const _Key, _Tp>.
     */
    template<typename _Key, typename _Tp,
            typename _Key_transform = key_transform<_Key>,
            typename _Alloc = std::allocator<std::pair<const _Key, _Tp> > >
    class radix_map {

    public:
        typedef _Key key_type;
        typedef _Tp mapped_type;
        typedef std::pair<const _Key, _Tp> value_type;
        typedef _Alloc allocator_type;

    private:
        // @TODO what is this?
        // concept requirements
        typedef typename _Alloc::value_type _Alloc_value_type;

        typedef Adaptive_radix_tree<key_type, mapped_type, _Key_transform> _Rep_type;

        _Rep_type _M_t;

    public:

        typedef typename _Rep_type::iterator iterator;
//        typedef typename _Rep_type::const_iterator const_iterator;
        typedef typename _Rep_type::size_type size_type;
        typedef typename _Rep_type::difference_type difference_type;
        typedef typename _Rep_type::reverse_iterator reverse_iterator;
//        typedef typename _Rep_type::const_reverse_iterator const_reverse_iterator;

        /**
         * @brief  Default constructor creates no elements.
         */
        radix_map() : _M_t() { }

        /**
         * @brief  Map copy constructor.
         */
        radix_map(const radix_map &__x) : _M_t(__x._M_t) { }

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
        radix_map(_InputIterator __first, _InputIterator __last)
                : _M_t() {
            _M_t._M_insert_unique(__first, __last);
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

        ///////////////
        // Modifiers //
        ///////////////

        /**
         *  Erases all elements in a %map.  Note that this function only
         *  erases the elements, and that if the elements themselves are
         *  pointers, the pointed-to memory is not touched in any way.
         *  Managing the pointer is the user's responsibility.
         */
        void clear() { _M_t.clear(); }

        /**
         *  @brief Attempts to insert a std::pair into the %map.

         *  @param __x Pair to be inserted (see std::make_pair for easy
         *	     creation of pairs).
         *
         *  @return  A pair, of which the first element is an iterator that
         *           points to the possibly inserted pair, and the second is
         *           a bool that is true if the pair was actually inserted.
         *
         *  This function attempts to insert a (key, value) %pair into the %map.
         *  A %map relies on unique keys and thus a %pair is only inserted if its
         *  first element (the key) is not already present in the %map.
         *
         *  Insertion requires O(k) time.
         */
        std::pair<iterator, bool> insert(const value_type &__x) {
            return _M_t._M_insert_unique(__x);
        }

        /**
         * @brief  Subscript ( @c [] ) access to %map data.
         * @param  __k  The key for which data should be retrieved.
         * @return  A reference to the data of the (key,data) %pair.
         *
         * Allows for easy lookup with the subscript ( @c [] )
         * operator.  Returns data associated with the key specified in
         * subscript.  If the key does not exist, a pair with that key
         * is created using default values, which is then returned.
         *
         * Lookup requires O(k) time.
         *
         */
        mapped_type &operator[](const key_type &__k) {
            // @TODO
            return NULL;
        }

        ////////////
        // Lookup //
        ////////////

        iterator find(const key_type &__x) {
            return _M_t.find(__x);
        }

        size_type count(const key_type &__x) const {
            return _M_t.find(__x) == _M_t.end() ? 0 : 1;
        }

        /**
         *  @brief  Access to %map data.
         *  @param  __k  The key for which data should be retrieved.
         *  @return  A reference to the data whose key is equivalent to @a __k, if
         *           such a data is present in the %map.
         *  @throw  std::out_of_range  If no such data is present.
         */
        mapped_type &at(const key_type &__k) {
            // @TODO
            return NULL;
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
    operator==(const radix_map<_Key, _Tp, _Key_transform> &__x,
               const radix_map<_Key, _Tp, _Key_transform> &__y) {
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
    operator<(const radix_map<_Key, _Tp, _Key_transform> &__x,
              const radix_map<_Key, _Tp, _Key_transform> &__y) {
        return __x._M_t < __y._M_t;
    }

    // Based on operator==
    template<typename _Key, typename _Tp, typename _Key_transform>
    inline bool
    operator!=(const radix_map<_Key, _Tp, _Key_transform> &__x,
               const radix_map<_Key, _Tp, _Key_transform> &__y) {
        return !(__x == __y);
    }

    // Based on operator<
    template<typename _Key, typename _Tp, typename _Key_transform>
    inline bool
    operator>(const radix_map<_Key, _Tp, _Key_transform> &__x,
              const radix_map<_Key, _Tp, _Key_transform> &__y) {
        return __y < __x;
    }

    // Based on operator<
    template<typename _Key, typename _Tp, typename _Key_transform>
    inline bool
    operator<=(const radix_map<_Key, _Tp, _Key_transform> &__x,
               const radix_map<_Key, _Tp, _Key_transform> &__y) {
        return !(__y < __x);
    }

    // Based on operator<
    template<typename _Key, typename _Tp, typename _Key_transform>
    inline bool
    operator>=(const radix_map<_Key, _Tp, _Key_transform> &__x,
               const radix_map<_Key, _Tp, _Key_transform> &__y) {
        return !(__x < __y);
    }
}

#endif //REFERENCE_ART_MAP_H
