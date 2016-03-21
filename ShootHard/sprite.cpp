#include "sprite.h"

#include <SDL2/SDL.h>

namespace sprite
{
    void init(Sprite& _sprite, SDL_Texture* _texture, int _layer /* = 0 */)
    {
        _sprite._texture = _texture;
        _sprite.position = glm::vec2(0.f, 0.f);
        _sprite.rotation = 0.f;
        _sprite.scale = glm::vec2(1.f, 1.f);
        _sprite.layer = _layer;

        SDL_QueryTexture(_texture, &_sprite._format, &_sprite._access, &_sprite._width, &_sprite._height);
    }

    void layer_sort_sprites(Sprite* _sprites, int _count)
    {

    }

    void render(Sprite& _sprite, SDL_Renderer* _renderer)
    {
        SDL_Rect r = SDL_Rect {
            (int)_sprite.position.x,
            (int)_sprite.position.y,
            (int)(_sprite.scale.x * _sprite._width),
            (int)(_sprite.scale.y * _sprite._height),
        };

        SDL_RenderCopyEx(_renderer, _sprite._texture, nullptr, &r, _sprite.rotation, nullptr, SDL_FLIP_NONE);
    }

    void render_sprites(SDL_Renderer* _renderer, Sprite* _sprites, int _count)
    {
        for (int i = 0; i < _count; ++i) {
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