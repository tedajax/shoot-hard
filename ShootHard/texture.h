#pragma once

#include "types.h"
#include "memory_types.h"

struct SDL_Renderer;

namespace texture
{
    void init_textures(foundation::Allocator& _textureAlloactor, SDL_Renderer* _renderer);
    void terminate_textures();

    const Texture& get(const char* _filename);

    bool load(Texture& _texture, SDL_Renderer* _renderer, const char* _filename);
    void unload(Texture& _texture);
    Texture get_default();
}