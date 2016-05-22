#include <SDL2/SDL.h>

#include "sprite.h"
#include "color.h"

namespace sprite
{
    void init(Sprite& _sprite, const Texture& _texture, glm::vec2 _origin /* = glm::vec2(0.5f, 0.5f) */, uint32 _layer /* = 0 */)
    {
        _sprite.texture = _texture;
        _sprite.layer = _layer;
        _sprite.origin = _origin;
        _sprite.color = color::cWhite;
    }
}