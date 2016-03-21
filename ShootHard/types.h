#pragma once

#include "basictypes.h"
#include <glm/vec2.hpp>

struct SDL_Texture;

struct Sprite
{
    SDL_Texture* _texture;
    int _width;
    int _height;
    int layer;
    glm::vec2 position;
    float32 rotation;
    glm::vec2 scale;
};
