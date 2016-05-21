#include "texture.h"
#include "hash.h"
#include "logger.h"

#include "memory.h"
#include "murmur_hash.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <cstring>

namespace texture
{
    struct TextureGlobals
    {
        foundation::Hash<Texture>* _table = nullptr;
        SDL_Renderer* _renderer = nullptr;
    };

    TextureGlobals _textures;

    namespace manager
    {
        void init(foundation::Allocator& _textureAlloactor, const Renderer& _renderer)
        {
            _textures._table = new foundation::Hash<Texture>(_textureAlloactor);
            _textures._renderer = _renderer._sdlRenderer;
        }

        void terminate()
        {
            for (auto itr = foundation::array::begin(_textures._table->_data); itr != foundation::array::end(_textures._table->_data); ++itr) {
                unload(itr->value);
            }

            delete _textures._table;
        }
    }

    const Texture& get(const char* _filename)
    {
        uint64 key = foundation::murmur_hash_64(_filename, (uint32)strlen(_filename), 0);

        if (foundation::hash::has(*_textures._table, key)) {
            return foundation::hash::get(*_textures._table, key, Texture());
        }

        Texture texture;
        if (load(texture, _filename)) {
            foundation::hash::set(*_textures._table, key, texture);
        }

        return foundation::hash::get(*_textures._table, key, Texture());
    }

    bool load(Texture& _texture, const char* _filename)
    {
        SDL_Surface* surface = IMG_Load(_filename);

        if (!surface) {
            logger::error("Texture", "Unable to open texture %s", _filename);
            return false;
        }

        _texture._sdlTexture = SDL_CreateTextureFromSurface(_textures._renderer, surface);

        SDL_QueryTexture(_texture._sdlTexture, nullptr, nullptr, &_texture.width, &_texture.height);

        SDL_FreeSurface(surface);

        return true;
    }

    void unload(Texture& _texture)
    {
        if (_texture._sdlTexture == nullptr) {
            return;
        }
        SDL_DestroyTexture(_texture._sdlTexture);
        _texture._sdlTexture = nullptr;
    }
}