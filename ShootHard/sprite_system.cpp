#include "component_entity.h"
#include "array.h"
#include "renderer.h"
#include "transform.h"

using namespace foundation::array;

namespace sprite_system
{
    void render(System<SpriteComponent>& _system, Renderer& _renderer)
    {
        for (int i = 0, len = size(_system._components); i < len; ++i) {
            auto transformHandle = component_entity::get_component<TransformComponent>(_system._components[i].entityId);
            renderer::add_draw_call(_renderer, RenderBuckets::cGameLayer, _system._components[i].sprite, transformHandle.get()->transform);
        }
    }
}