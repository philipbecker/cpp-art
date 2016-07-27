#ifndef ART_RADIX_SET_H
#define ART_RADIX_SET_H

#include "art/adaptive_radix_tree.h"

namespace art
{
    template<typename _Key,
            typename _Key_transform = key_transform <_Key> >
    class radix_set {
        typedef _Key key_type;
        typedef _Key value_type;

        typedef adaptive_radix_tree<key_type, value_type, _Key_transform> _Rep_type;
        _Rep_type _M_t;

    };
}

#endif //ART_RADIX_SET_H
