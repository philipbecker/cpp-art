#ifndef ART_RADIX_SET_H
#define ART_RADIX_SET_H

#include <set>

namespace art
{
    template<typename _Key,
            typename _Key_transform = key_transform <_Key>,
            typename _Alloc = std::allocator<std::pair<_Key> > >
    class radix_set {
        typedef _Key key_type;
        typedef _Key value_type;

        typedef Adaptive_radix_tree <key_type, value_type, _Key_transform> _Rep_type;
        _Rep_type _M_t;

    };
}

#endif //ART_RADIX_SET_H
