#pragma once

#include "basictypes.h"
#include "collection_types.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

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
    uint32 layer = 0;
    Color color;
    glm::vec2 position;
    float32 rotation;
    glm::vec2 scale;
};

struct Mesh
{
    foundation::Array<glm::vec3>* vertices;
    foundation::Array<glm::vec3>* normals;
    foundation::Array<glm::vec2>* uvs;
    foundation::Array<glm::vec3>* colors;
    byte _buffer[sizeof(foundation::Array<glm::vec3>) * 4];

    Mesh();
    ~Mesh();
};

enum class ShaderType
{
    cVertex,
    cFragment
};

struct Shader
{
    ShaderType _shaderType;
    uint _shaderId = 0;
};

struct Material
{
    uint _programId = 0;
    foundation::Hash<int>* _uniforms;
};