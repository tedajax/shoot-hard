#pragma once

#include "types.h"
#include "array.h"
#include "texture.h"

struct SDL_Renderer;

namespace sprite
{
    void init(Sprite& _sprite, const Texture& _texture, uint32 _layer = 0);
    Sprite create(const Texture& _texture, uint32 _layer = 0);
}