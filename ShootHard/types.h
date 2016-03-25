#pragma once

#include "debug.h"
#include "basictypes.h"
#include "collection_types.h"
#include "memory_types.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>

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
    foundation::Array<glm::vec3>* vertices = nullptr;
    foundation::Array<glm::vec3>* normals = nullptr;
    foundation::Array<glm::vec2>* uvs = nullptr;
    foundation::Array<glm::vec4>* colors = nullptr;
    foundation::Array<uint32>* indices = nullptr;

    byte _buffer[sizeof(foundation::Array<glm::vec3>) * 5];

    Mesh();
    Mesh(foundation::Allocator& _a);
    ~Mesh();
};

struct MeshInstance
{
    uint vertexBuffer = 0;
    uint normalBuffer = 0;
    uint uvBuffer = 0;
    uint colorBuffer = 0;
    uint indexBuffer = 0;
    uint32 indexCount = 0;
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

struct Rectangle
{
    glm::vec2 position = glm::vec2(0, 0);
    float32 width = 0;
    float32 height = 0;
};

enum class ProjectionType
{
    cPerspective,
    cOrtho,
};

struct Camera
{
    glm::vec3 position;
    glm::vec3 lookAt;
    glm::vec3 up;

    ProjectionType projectionType;
    float32 fov;
    float32 aspectRatio;
    float32 orthoSize;
    float32 nearZ;
    float32 farZ;
};