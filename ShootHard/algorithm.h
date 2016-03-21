#pragma once

#include "basictypes.h"

namespace internal
{
    template <typename T> inline void array_sort_section(T* _data, uint32 _i, uint32 _count, bool(*_lessThanFunc)(const T* e1, const T* e2))
    {
        T hold = _data[_i];

        uint32 k = _i * 2 + 1;
        while (k < _count) {
            if (k + 1 < _count && _lessThanFunc(&_data[k], &_data[k + 1])) {
                ++k;
            }

            if (!_lessThanFunc(&hold, &_data[k])) {
                break;
            }

            _data[_i] = _data[k];
            _i = k;
            k = _i * 2 + 1;
        }

        _data[_i] = hold;
    }
}

template <typename T> inline void array_sort_count(T* _data, uint32 _count, bool(*_lessThanFunc)(const T* e1, const T* e2))
{
    uint32 i = _count / 2;
    while (i-- > 0) {
        internal::array_sort_section(_data, i, _count, _lessThanFunc);
    }

    while (--_count > 0) {
        T temp = _data[0];
        _data[0] = _data[_count];
        _data[_count] = temp;
        internal::array_sort_section(_data, 0, _count, _lessThanFunc);
    }
}
