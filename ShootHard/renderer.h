#pragma once

#include "types.h"

namespace renderer
{
    void init(Renderer& _renderer, const Window& _window, foundation::Allocator& _allocator);
    void shutdown(Renderer& _renderer);

    void set_draw_color(Renderer& _renderer, const Color& _color);
    void clear(Renderer& _renderer);
    void copy(Renderer& _renderer, const Texture& _texture, const Rectangle* _srcRect, const Rectangle* _destRect);
    void present(Renderer& _renderer);
}