#pragma once

#include "types.h"

struct SDL_Renderer;

namespace sprite
{
    void init(Sprite& _sprite, SDL_Texture* _texture, int _layer = 0);
    void layer_sort_sprites(Sprite* _sprites, int _count);
    void render(Sprite& _sprite, SDL_Renderer* _renderer);
    void render_sprites(SDL_Renderer* _renderer, Sprite* _sprites, int _count);
}