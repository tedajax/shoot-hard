#include <SDL2/SDL.h>

#include "sprite.h"
#include "color.h"

namespace sprite
{
    void init(Sprite& _sprite, const Texture& _texture, uint32 _layer /* = 0 */)
    {
        _sprite.texture = _texture;
        _sprite.layer = _layer;
    }

    Sprite create(const Texture& _texture, uint32 _layer /* = 0 */)
    {
        Sprite result;
        init(result, _texture, _layer);
        return result;
    }
}