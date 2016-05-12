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

        _texture.width = (float32)surface->w;
        _texture.height = (float32)surface->h;
        _texture.pixelCount = surface->w * surface->h;
        _texture.size = surface->pitch * surface->h;
        _texture.pixels = new byte[_texture.size];

        // todo: be smarter
        _texture.format = TextureFormat::cRGBA;

        std::memcpy(_texture.pixels, surface->pixels, _texture.size);

        SDL_FreeSurface(surface);

        glGenTextures(1, &_texture.textureId);

        bind(_texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int)_texture.width, (int)_texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _texture.pixels);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        // todo
        // generate texture
        // copy data into gl texture
        // store stuff

        return true;
    }

    void unload(Texture& _texture)
    {
        if (_texture.pixels) {
            delete _texture.pixels;
        }

        if (_texture.textureId != 0xFFFFFFFF) {
            glDeleteTextures(1, &_texture.textureId);
        }
    }

    void bind(const Texture& _texture)
    {
        if (_texture.textureId != 0xFFFFFFFF) {
            glBindTexture(GL_TEXTURE_2D, _texture.textureId);
        }
    }
}