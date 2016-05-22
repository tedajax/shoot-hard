#pragma once

#include "component.h"

struct TransformComponent : public Component
{
    Transform transform;
};

struct SpriteComponent : public Component
{
    Sprite sprite;
};