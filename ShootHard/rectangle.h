#pragma once

#include "types.h"

namespace rectangle
{
    Rectangle default_uvs();

    inline float32 top(const Rectangle& _rectangle)
    {
        return _rectangle.position.y;
    }

    inline float32 bottom(const Rectangle& _rectangle)
    {
        return _rectangle.position.y + _rectangle.height;
    }

    inline float32 left(const Rectangle& _rectangle)
    {
        return _rectangle.position.x;
    }

    inline float32 right(const Rectangle& _rectangle)
    {
        return _rectangle.position.x + _rectangle.width;
    }
}