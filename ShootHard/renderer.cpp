#include "renderer.h"
#include "queue.h"
#include "rectangle.h"

#include <SDL2/SDL.h>

namespace renderer
{
    void init(Renderer& _renderer, const Window& _window, foundation::Allocator& _allocator)
    {
        ASSERT(_renderer._sdlRenderer == nullptr, "Renderer is already initialized.");
        
        _renderer._sdlRenderer = SDL_CreateRenderer(_window._sdlWindow, -1, SDL_RENDERER_ACCELERATED);

        for (int i = 0; i < (int)RenderBuckets::cCount; ++i) {
            _renderer._renderBuckets[i] = new foundation::Queue<RenderCommand>(_allocator);
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

    void copy(Renderer& _renderer, const Texture& _texture, const Rectangle* _srcRect, const Rectangle* _dstRect)
    {
        ASSERT(_renderer._sdlRenderer != nullptr, "");

        SDL_Rect* src = nullptr;
        SDL_Rect* dst = nullptr;
        rectangle::make_sdl_rect(_srcRect, &src);
        rectangle::make_sdl_rect(_dstRect, &dst);

        SDL_RenderCopy(_renderer._sdlRenderer, _texture._sdlTexture, src, dst);
    }

    void present(Renderer& _renderer)
    {
        ASSERT(_renderer._sdlRenderer != nullptr, "");

        SDL_RenderPresent(_renderer._sdlRenderer);
    }
}