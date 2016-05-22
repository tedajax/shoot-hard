#include "rectangle.h"

#include <SDL2/SDL.h>

namespace rectangle
{
    Rectangle default_uvs()
    {
        Rectangle r;
        r.position.x = 0.f;
        r.position.y = 0.f;
        r.width = 1.f;
        r.height = 1.f;
        return r;
    }

    void make_sdl_rect(const Rectangle* _rectangle, SDL_Rect** _sdlRect)
    {
        if (_rectangle == nullptr) {
            *_sdlRect = nullptr;
            return;
        }

        (*_sdlRect)->x = (int)_rectangle->position.x;
        (*_sdlRect)->y = (int)_rectangle->position.y;
        (*_sdlRect)->w = (int)_rectangle->width;
        (*_sdlRect)->h = (int)_rectangle->height;
    }

    void create_from_transform(const Transform& _transform, float32 _width, float32 _height, Rectangle& _out)
    {
        _out.position.x = _transform.position.x;
        _out.position.y = _transform.position.y;
        _out.width = _width * _transform.scale.x;
        _out.height = _height * _transform.scale.y;
    }
}