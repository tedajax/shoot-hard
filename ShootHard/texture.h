#pragma once

#include "types.h"
#include "memory_types.h"

namespace texture
{
    namespace manager
    {
        void init(foundation::Allocator& _textureAlloactor);
        void terminate();
    }

    const Texture& get(const char* _filename);

    bool load(Texture& _texture, const char* _filename);
    void unload(Texture& _texture);
    Texture get_default();

    void bind(const Texture& _texture);
}