#pragma once

#include "types.h"

namespace renderer
{
    void init(Renderer& _renderer, const Window& _window, foundation::Allocator& _allocator);
    void shutdown(Renderer& _renderer);

    void set_draw_color(Renderer& _renderer, const Color& _color);
    void clear(Renderer& _renderer);
    void render(Renderer& _renderer);

    void add_draw_call(Renderer& _renderer, RenderBuckets bucket, const Sprite& _sprite, const Transform& _transform);
    void render_sprite(Renderer& _renderer, Sprite& _sprite, const Transform& _transform);
}