#include <SDL2/SDL.h>

#include "sprite.h"

namespace sprite
{
    void init(Sprite& _sprite, SDL_Texture* _texture, int _layer /* = 0 */)
    {
        _sprite._texture = _texture;
        _sprite.position = glm::vec2(0.f, 0.f);
        _sprite.rotation = 0.f;
        _sprite.scale = glm::vec2(1.f, 1.f);
        _sprite.layer = _layer;

        SDL_QueryTexture(_texture, nullptr, nullptr, &_sprite._width, &_sprite._height);
    }

    void layer_sort_sprites(foundation::Array<Sprite>& _sprites)
    {
        const auto layerLessThanFunc = [](const Sprite* _s1, const Sprite* _s2) {
            return _s1->layer < _s2->layer;
        };

        foundation::array::sort<Sprite>(_sprites, layerLessThanFunc);
    }

    void render_sprites(SDL_Renderer* _renderer, foundation::Array<Sprite>& _sprites)
    {
        for (int i = 0, len = foundation::array::size(_sprites); i < len; ++i) {
            SDL_Rect r = SDL_Rect{
                (int)_sprites[i].position.x,
                (int)_sprites[i].position.y,
                (int)(_sprites[i].scale.x * _sprites[i]._width),
                (int)(_sprites[i].scale.y * _sprites[i]._height),
            };

            SDL_RenderCopyEx(_renderer, _sprites[i]._texture, nullptr, &r, _sprites[i].rotation, nullptr, SDL_FLIP_NONE);
        }
    }
}