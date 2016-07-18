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
            typename _Key_transform = key_transform <_Key>,
            typename _Alloc = std::allocator<std::pair<const _Key, _Tp> > >
    class radix_map {

    public:
        typedef _Key key_type;
        typedef _Tp mapped_type;
        typedef std::pair<const _Key, _Tp> value_type;
        typedef _Key_transform key_transformer;
        typedef _Alloc allocator_type;
        typedef value_type &reference;
        typedef const value_type &const_reference;

    private:
        // @TODO what is this?
        // concept requirements
        typedef typename _Alloc::value_type _Alloc_value_type;

        typedef Adaptive_radix_tree <key_type, mapped_type, _Key_transform> _Rep_type;

        _Rep_type _M_t;

    public:

        //typedef typename _Alloc_traits::pointer            pointer;
        //typedef typename _Alloc_traits::const_pointer      const_pointer;
        //typedef typename _Alloc_traits::reference          reference;
        //typedef typename _Alloc_traits::const_reference    const_reference;
        // Bidiretional iterator
        typedef typename _Rep_type::iterator iterator;
        typedef typename _Rep_type::const_iterator const_iterator;
        typedef typename _Rep_type::size_type size_type;
        typedef typename _Rep_type::difference_type difference_type;
        typedef typename _Rep_type::reverse_iterator reverse_iterator;
        typedef typename _Rep_type::const_reverse_iterator const_reverse_iterator;

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

        /**
         *  @brief  Builds a %map from an initializer_list.
         *  @param  __l  An initializer_list.
         *  @param  __comp  A comparison object.
         *  @param  __a  An allocator object.
         *
         *  Create a %map consisting of copies of the elements in the
         *  initializer_list @a __l.
         *  This is linear in N if the range is already sorted, and NlogN
         *  otherwise (where N is @a __l.size()).
         */
        radix_map(std::initializer_list<value_type> __l,
                  const _Key_transform &__key_transformer = _Key_transform(),
                  const allocator_type &__a = allocator_type())
                : _M_t(__key_transformer /*, _Pair_alloc_type(__a) */) {
            _M_t._M_insert_unique(__l.begin(), __l.end());
        }

        /**
         *  @brief  %Map assignment operator.
         *  @param  __x  A %map of identical element and allocator types.
         *
         *  All the elements of @a __x are copied, but unlike the copy
         *  constructor, the allocator object is not copied.
         */
        radix_map &operator=(const radix_map &__x) {
            // @TODO implement art assignment operator
            _M_t = __x._M_t;
            return *this;
        }

        /// Move assignment operator.
        // @TODO implement art move assignment operator
        radix_map &operator=(radix_map &&) = default;

        /**
         *  @brief  %Map list assignment operator.
         *  @param  __l  An initializer_list.
         *
         *  This function fills a %map with copies of the elements in the
         *  initializer list @a __l.
         *
         *  Note that the assignment completely changes the %map and
         *  that the resulting %map's size is the same as the number
         *  of elements assigned.  Old data may be lost.
         */
        radix_map &operator=(std::initializer_list<value_type> __l) {
            _M_t._M_assign_unique(__l.begin(), __l.end());
            return *this;
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
         *  @brief Template function that attempts to insert a range of elements.
         *  @param  __first  Iterator pointing to the start of the range to be
         *                   inserted.
         *  @param  __last  Iterator pointing to the end of the range.
         *
         *  Complexity similar to that of the range constructor.
         */
        template<typename _InputIterator>
        void insert(_InputIterator __first, _InputIterator __last) {
            _M_t._M_insert_unique(__first, __last);
        }

        /**
         *  @brief Attempts to insert a list of std::pairs into the %map.
         *  @param  __list  A std::initializer_list<value_type> of pairs to be
         *                  inserted.
         *
         *  Complexity similar to that of the range constructor.
         */
        void insert(std::initializer_list<value_type> __list) {
            insert(__list.begin(), __list.end());
        }

        template<typename... _Args>
        std::pair<iterator, bool> emplace(_Args &&... __args) {
            throw;
            //return _M_t._M_emplace_unique(std::forward<_Args>(__args)...);
        }

        size_type erase(const key_type &__k) {
            return _M_t.erase_unique(__k);
        }

        // @TODO implementation
        iterator erase(const_iterator __it) {
            throw;

            return NULL; // successor
        }

        // @TODO implementation
        iterator erase(const_iterator __first, const_iterator __last) {
            for (; __first != __last; ++__first)
                _M_t.erase_unique(__first);

            return NULL; // successor
        }

        /**
         *  @brief  Swaps data with another %map.
         *  @param  __x  A %map of the same element and allocator types.
         *
         *  This exchanges the elements between two maps in constant
         *  time.
         */
        void swap(radix_map &__x) {
            _M_t.swap(__x._M_t);
        }


        ////////////////////
        // Element access //
        ////////////////////

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
            // @TODO more efficient to write extra method to avoid 2 lookups
            iterator it = _M_t.find(__k);
            if (it != _M_t.end())
                return (*it).second;

            std::pair<iterator, bool> res = _M_t._M_insert_unique(
                    std::pair<_Key, _Tp>(__k, mapped_type()));
            return (*res.first).second;
        }

        /**
         *  @brief  Access to %map data.
         *  @param  __k  The key for which data should be retrieved.
         *  @return  A reference to the data whose key is equivalent to @a __k, if
         *           such a data is present in the %map.
         *  @throw  std::out_of_range  If no such data is present.
         */
        mapped_type &at(const key_type &__k) {
            iterator res = _M_t.find(__k);
            if (res == _M_t.end())
                std::__throw_out_of_range("radix_map::at");
            return (*res).second;
        }

        ////////////
        // Lookup //
        ////////////

        // @TODO requires const find method & const iterator
        size_type count(const key_type &__x) const {
            return _M_t.find(__x) == _M_t.end() ? 0 : 1;
        }

        /**
         *  @brief Tries to locate an element in a %map.
         *  @param  __k  Key of (key, value) %pair to be located.
         *  @return  Iterator pointing to sought-after element, or end() if not
         *           found.
         *
         *  This function takes a key and tries to locate the element with which
         *  the key matches.  If successful the function returns an iterator
         *  pointing to the sought after %pair.  If unsuccessful it returns the
         *  past-the-end ( @c end() ) iterator.
         */
        iterator find(const key_type &__k) {
            return _M_t.find(__k);
        }

        /**
         *  @brief Finds the beginning of a subsequence matching given key.
         *  @param  __k  Key of (key, value) pair to be located.
         *  @return  Iterator pointing to first element equal to or greater
         *           than key, or end().
         *
         *  This function returns the first element of a subsequence of elements
         *  that matches the given key.  If unsuccessful it returns an iterator
         *  pointing to the first element that has a greater value than given key
         *  or end() if no such element exists.
         */
        iterator lower_bound(const key_type &__k) {
            return _M_t.lower_bound(__k);
        }

        const_iterator lower_bound(const key_type &__k) const {
            return _M_t.lower_bound(__k);
        }

        /**
         *  @brief Finds the end of a subsequence matching given key.
         *  @param  __k  Key of (key, value) pair to be located.
         *  @return Iterator pointing to the first element
         *          greater than key, or end().
         */
        iterator upper_bound(const key_type &__k) {
            return _M_t.upper_bound(__k);
        }

        const_iterator upper_bound(const key_type &__k) const {
            return _M_t.upper_bound(__k);
        }

        ///////////////
        // Iterators //
        ///////////////

        iterator begin() {
            return _M_t.begin();
        }

        const_iterator begin() const {
            return _M_t.begin();
        }

        const_iterator cbegin() const {
            return _M_t.begin();
        }

        iterator end() {
            return _M_t.end();
        }

        const_iterator end() const {
            return _M_t.end();
        }

        const_iterator cend() const {
            return _M_t.end();
        }

        reverse_iterator rbegin() {
            return _M_t.rbegin();
        }

        const_reverse_iterator rbegin() const {
            return _M_t.rbegin();
        }

        reverse_iterator rend() {
            return _M_t.rend();
        }

        const_reverse_iterator rend() const {
            return _M_t.rend();
        }

        ///////////////
        // Observers //
        ///////////////

        _Key_transform key_trans() const {
            return _M_t.key_trans();
        }
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

    // See radix_map::swap()
    template<typename _Key, typename _Tp, typename _Key_transform>
    inline bool
    swap(radix_map<_Key, _Tp, _Key_transform> &__x,
         radix_map<_Key, _Tp, _Key_transform> &__y) {
        __x.swap(__y);
    }
}

#endif //REFERENCE_ART_MAP_H
