#pragma once

#include "debug.h"
#include "basic_types.h"
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

struct SDL_Texture;
struct Texture
{
    SDL_Texture* _sdlTexture = nullptr;
    int width;
    int height;
};

struct Sprite
{
    Texture texture;
    uint32 layer = 0;
    glm::vec2 origin = glm::vec2(0.5f, 0.5f);
    Color color;
};

struct Transform
{
    glm::vec2 position = glm::vec2(0.f, 0.f);
    glm::vec2 scale = glm::vec2(1.f, 1.f);
    float32 rotation = 0.f;
};

struct Rectangle
{
    glm::vec2 position;
    float32 width;
    float32 height;
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

struct RenderCall
{
    Sprite sprite;
    Transform transform;
};

enum class RenderBuckets
{
    cGameLayer,
    cHudLayer,
    cCount,
};

struct SDL_Renderer;
struct Renderer
{
    SDL_Renderer* _sdlRenderer = nullptr;
    foundation::Array<RenderCall>* _renderBuckets[(int)RenderBuckets::cCount];
};

enum class WindowStyle
{
    cWindowed,
    cFullscreen,
    cBorderlessWindow,
};

struct SDL_Window;
struct Window
{
    SDL_Window* _sdlWindow = nullptr;
    WindowStyle _style = WindowStyle::cWindowed;
    int _width = 0;
    int _height = 0;
};