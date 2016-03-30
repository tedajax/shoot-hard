#pragma once

#include "debug.h"
#include "basictypes.h"
#include "collection_types.h"
#include "memory_types.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/quaternion.hpp>

struct Color
{
    uint8 a = 255, r = 255, g = 255, b = 255;
};

enum class TextureFormat
{
    cRGBA,
    cARGB,
    cRGB,
    cRG,
    cR,
};

enum class TextureWrapMode
{
    cRepeat,
    cClamp,
};

enum class TextureFilterMode
{
    cNearest,
    cBilinear,
    cTrilinear,
};

struct Texture
{
    uint32 _textureId = 0xFFFFFFFF;
    float32 _width = 0.f;
    float32 _height = 0.f;
    TextureFormat _format = TextureFormat::cRGBA;
    TextureWrapMode _wrapMode = TextureWrapMode::cRepeat;
    TextureFilterMode _filterMode = TextureFilterMode::cNearest;
    uint32 _size = 0;
    uint32 _pixelCount = 0;
    byte* _pixels = nullptr;
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
    glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);
    glm::quat rotation = glm::quat();

    ProjectionType projectionType;
    float32 fov;
    float32 aspectRatio;
    float32 orthoSize;
    float32 nearZ;
    float32 farZ;
};