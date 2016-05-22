#pragma once

#include "types.h"
#include "array.h"
#include "texture.h"

struct SDL_Renderer;

namespace sprite
{
    void init(Sprite& _sprite, const Texture& _texture, glm::vec2 _origin = glm::vec2(0.5f, 0.5f), uint32 _layer = 0);
}