#pragma once

#include "types.h"

struct SDL_Rect;
namespace rectangle
{
    Rectangle default_uvs();
    void make_sdl_rect(const Rectangle* _rectangle, SDL_Rect** _sdlRect);
    void create_from_transform(const Transform& _transform, float32 _width, float32 _height, Rectangle& _out);

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