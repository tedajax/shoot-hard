#include "renderer.h"
#include "queue.h"
#include "array.h"
#include "rectangle.h"
#include "texture.h"
#include "sprite.h"
#include "color.h"

#include <SDL2/SDL.h>

namespace renderer
{
    void init(Renderer& _renderer, const Window& _window, foundation::Allocator& _allocator)
    {
        ASSERT(_renderer._sdlRenderer == nullptr, "Renderer is already initialized.");
        
        _renderer._sdlRenderer = SDL_CreateRenderer(_window._sdlWindow, -1, SDL_RENDERER_ACCELERATED);

        for (int i = 0; i < (int)RenderBuckets::cCount; ++i) {
            _renderer._renderBuckets[i] = new foundation::Array<RenderCall>(_allocator);
            foundation::array::reserve(*_renderer._renderBuckets[i], 256);
        }
    }

    void shutdown(Renderer& _renderer)
    {
        ASSERT(_renderer._sdlRenderer != nullptr, "Renderer is not initialized.");

        SDL_DestroyRenderer(_renderer._sdlRenderer);

        for (int i = 0; i < (int)RenderBuckets::cCount; ++i) {
            delete _renderer._renderBuckets[i];
            
        }
    }

    void set_draw_color(Renderer& _renderer, const Color& _color)
    {
        ASSERT(_renderer._sdlRenderer != nullptr, "");

        SDL_SetRenderDrawColor(_renderer._sdlRenderer, _color.r, _color.g, _color.b, _color.a);
    }

    void clear(Renderer& _renderer)
    {
        ASSERT(_renderer._sdlRenderer != nullptr, "");

        SDL_RenderClear(_renderer._sdlRenderer);
    }

    void render(Renderer& _renderer)
    {
        ASSERT(_renderer._sdlRenderer != nullptr, "");

        const auto layerLessThanFunc = [](const RenderCall* _s1, const RenderCall* _s2) {
            return (_s1->sprite.layer > _s2->sprite.layer) ||
                ((_s1->sprite.layer == _s2->sprite.layer) && (color::pack(_s1->sprite.color) < color::pack(_s2->sprite.color)));
        };

        for (int i = 0; i < (int)RenderBuckets::cCount; ++i) {
            auto& bucket = *_renderer._renderBuckets[i];
            foundation::array::sort<RenderCall>(bucket, layerLessThanFunc);

            for (int j = 0, len = foundation::array::size(bucket); j < len; ++j) {
                render_sprite(_renderer, bucket[j].sprite, bucket[j].transform);
            }

            foundation::array::clear(bucket);
        }

        SDL_RenderPresent(_renderer._sdlRenderer);
    }

    void add_draw_call(Renderer& _renderer, RenderBuckets bucket, const Sprite& _sprite, const Transform& _transform)
    {
        foundation::Array<RenderCall>* renderQueue = _renderer._renderBuckets[(int)bucket];
        RenderCall renderCall = {
            _sprite,
            _transform
        };
        foundation::array::push_back(*renderQueue, renderCall);
    }

    void render_sprite(Renderer& _renderer, Sprite& _sprite, const Transform& _transform)
    {
        SDL_Rect r;
        r.x = (int)_transform.position.x;
        r.y = (int)_transform.position.y;
        r.w = (int)(_sprite.texture.width * _transform.scale.x);
        r.h = (int)(_sprite.texture.height * _transform.scale.y);

        texture::set_color(_sprite.texture, _sprite.color);
        
        SDL_Point center;
        center.x = (int)(_sprite.origin.x * _sprite.texture.width);
        center.y = (int)(_sprite.origin.y * _sprite.texture.height);

        SDL_RenderCopyEx(_renderer._sdlRenderer, _sprite.texture._sdlTexture, nullptr, &r, _transform.rotation, &center, SDL_FLIP_NONE);
    }
}