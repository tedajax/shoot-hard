#pragma once

#include "types.h"

namespace math
{
    inline float32 min(float32 _a, float32 _b)
    {
        return (_a < _b) ? _a : _b;
    }

    inline float32 max(float32 _a, float32 _b)
    {
        return (_a > _b) ? _a : _b;
    }
    
    inline float32 clamp(float32 _value, float32 _min, float32 _max)
    {
        return max(min(_value, _max), _min);
    }

    inline float32 lerp(float32 _a, float32 _b, float32 _t)
    {
        return (_b - _a) * _t + _a;
    }
}