#pragma once

#include "types.h"
#include "math.h"

namespace color
{
    inline Color create(uint8 _r, uint8 _g, uint8 _b, uint8 _a = 255)
    {
        Color result;
        result.r = _r;
        result.g = _g;
        result.b = _b;
        result.a = _a;
        return result;
    }

    inline Color create(int _r, int _g, int _b, int _a = 255)
    {
        return create((uint8)_r, (uint8)_g, (uint8)_b, (uint8)_a);
    }

    inline Color create(float32 _r, float32 _g, float32 _b, float32 _a = 1.f)
    {
        uint8 r = (uint8)math::clamp(_r, 0.f, 1.f) * 255;
        uint8 g = (uint8)math::clamp(_g, 0.f, 1.f) * 255;
        uint8 b = (uint8)math::clamp(_b, 0.f, 1.f) * 255;
        uint8 a = (uint8)math::clamp(_a, 0.f, 1.f) * 255;
        return create(r, g, b, a);
    }

    inline uint32 pack(const Color _color)
    {
        return (_color.a << 24) | (_color.r << 16) | (_color.g << 8) | _color.b;
    }

    inline Color unpack(uint32 _packedColor)
    {
        return create(
            (uint8)((_packedColor & 0xFF000000) >> 24),
            (uint8)((_packedColor & 0x00FF0000) >> 16),
            (uint8)((_packedColor & 0x0000FF00) >> 8),
            (uint8)(_packedColor & 0x000000FF)
        );
    }
}