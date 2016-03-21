#include "texture.h"
#include "hash.h"

#include "memory.h"
#include "murmur_hash.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <cstring>

namespace texture
{
    struct TextureGlobals
    {
        SDL_Renderer* renderer = nullptr;
        foundation::Hash<Texture>* _table = nullptr;
    };

    TextureGlobals _textures;

    namespace manager
    {
        void init(foundation::Allocator& _textureAlloactor, SDL_Renderer* _renderer)
        {
            _textures._table = new foundation::Hash<Texture>(_textureAlloactor);
            _textures.renderer = _renderer;
        }

        void terminate()
        {
            for (auto itr = foundation::array::begin(_textures._table->_data); itr != foundation::array::end(_textures._table->_data); ++itr) {
                if (itr->value._texture) {
                    SDL_DestroyTexture(itr->value._texture);
                }
            }

            delete _textures._table;
        }
    }

    const Texture& get(const char* _filename)
    {
        uint64 key = foundation::murmur_hash_64(_filename, (uint32)strlen(_filename), 0);

        if (foundation::hash::has(*_textures._table, key)) {
            return foundation::hash::get(*_textures._table, key, get_default());
        }

        Texture texture;
        if (load(texture, _textures.renderer, _filename)) {
            foundation::hash::set(*_textures._table, key, texture);
        }

        return foundation::hash::get(*_textures._table, key, get_default());
    }

    bool load(Texture& _texture, SDL_Renderer* _renderer, const char* _filename)
    {
        _texture._texture = nullptr;
        _texture._width = 0;
        _texture._height = 0;

        SDL_Surface* surface = IMG_Load(_filename);

        if (!surface) {
            return false;
        }

        _texture._texture = SDL_CreateTextureFromSurface(_renderer, surface);

        SDL_FreeSurface(surface);

        if (!_texture._texture) {
            return false;
        }

        SDL_QueryTexture(_texture._texture, nullptr, nullptr, &_texture._width, &_texture._height);

        return true;
    }

    void unload(Texture& _texture)
    {
        if (_texture._texture) {
            SDL_DestroyTexture(_texture._texture);
        }
    }

    Texture get_default()
    {
        return Texture{
            nullptr,
            0,
            0
        };
    }
}