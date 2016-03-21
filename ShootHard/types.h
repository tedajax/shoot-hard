#pragma once

#include "basictypes.h"
#include <glm/vec2.hpp>

struct Color
{
    uint8 a = 255, r = 255, g = 255, b = 255;
};

struct SDL_Texture;

struct Texture
{
    SDL_Texture* _texture;
    int _width;
    int _height;
};

struct Sprite
{
    Texture _texture;
    uint32 layer;
    Color color;
    glm::vec2 position;
    float32 rotation;
    glm::vec2 scale;
};