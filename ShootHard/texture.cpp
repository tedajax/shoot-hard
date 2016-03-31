#include "texture.h"
#include "hash.h"

#include "memory.h"
#include "murmur_hash.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <gl/glew.h>

#include <cstring>

namespace texture
{
    struct TextureGlobals
    {
        foundation::Hash<Texture>* _table = nullptr;
    };

    TextureGlobals _textures;

    namespace manager
    {
        void init(foundation::Allocator& _textureAlloactor)
        {
            _textures._table = new foundation::Hash<Texture>(_textureAlloactor);
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
            return false;
        }

        _texture._width = (float32)surface->w;
        _texture._height = (float32)surface->h;
        _texture._pixelCount = surface->w * surface->h;
        _texture._size = surface->pitch * surface->h;
        _texture._pixels = new byte[_texture._size];

        // todo: be smarter
        _texture._format = TextureFormat::cRGBA;

        std::memcpy(_texture._pixels, surface->pixels, _texture._size);

        SDL_FreeSurface(surface);

        glGenTextures(1, &_texture._textureId);

        bind(_texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _texture._width, _texture._height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _texture._pixels);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        // todo
        // generate texture
        // copy data into gl texture
        // store stuff

        return true;
    }

    void unload(Texture& _texture)
    {
        if (_texture._pixels) {
            delete _texture._pixels;
        }

        if (_texture._textureId != 0xFFFFFFFF) {
            glDeleteTextures(1, &_texture._textureId);
        }
    }

    void bind(const Texture& _texture)
    {
        if (_texture._textureId != 0xFFFFFFFF) {
            glBindTexture(GL_TEXTURE_2D, _texture._textureId);
        }
    }
}