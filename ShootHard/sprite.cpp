#include <SDL2/SDL.h>

#include "sprite.h"
#include "color.h"

namespace sprite
{
    void init(Sprite& _sprite, const Texture& _texture, uint32 _layer /* = 0 */)
    {
        _sprite.texture = _texture;
        _sprite.position = glm::vec2(0.f, 0.f);
        _sprite.rotation = 0.f;
        _sprite.scale = glm::vec2(1.f, 1.f);
        _sprite.layer = _layer;
    }

    Sprite create(const Texture& _texture, uint32 _layer /* = 0 */)
    {
        Sprite result;
        init(result, _texture, _layer);
        return result;
    }

    void layer_sort_sprites(foundation::Array<Sprite>& _sprites)
    {
        const auto layerLessThanFunc = [](const Sprite* _s1, const Sprite* _s2) {
            return (_s1->layer > _s2->layer) ||
                ((_s1->layer == _s2->layer) && (color::pack(_s1->color) < color::pack(_s2->color)));
        };

        foundation::array::sort<Sprite>(_sprites, layerLessThanFunc);
    }

    void render_sprites(SDL_Renderer* _renderer, foundation::Array<Sprite>& _sprites)
    {
        // for (int i = 0, len = foundation::array::size(_sprites); i < len; ++i) {
        //     Sprite sprite = _sprites[i];
        //     SDL_Rect r = SDL_Rect{
        //         (int)sprite.position.x,
        //         (int)sprite.position.y,
        //         (int)(sprite.scale.x * sprite._texture._width),
        //         (int)(sprite.scale.y * sprite._texture._height),
        //     };

        //     SDL_SetTextureColorMod(sprite._texture._texture, sprite.color.r, sprite.color.g, sprite.color.b);
        //     SDL_RenderCopyEx(_renderer, sprite._texture._texture, nullptr, &r, sprite.rotation, nullptr, SDL_FLIP_NONE);
        // }
    }
}