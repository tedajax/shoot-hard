#pragma once

#include "system.h"
#include "component_types.h"

namespace sprite_system
{
    void render(System<SpriteComponent>& _system, Renderer& _renderer);
}