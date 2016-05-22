#pragma once

#include "entity.h"
#include "component.h"
#include "component_types.h"
#include "system.h"
#include "sprite_system.h"
#include <unordered_map>

namespace component_entity
{
    namespace internal
    {
        const int cMaxRegisteredSystems = 32;

        static const std::type_info* s_typeInfoKeys[cMaxRegisteredSystems];
        static void* s_systemValues[cMaxRegisteredSystems];

        static int s_currentSystemIndex = 0;
    }

    inline void init()
    {
    }

    inline void shutdown()
    {
    }

    template <typename T_COMPONENT>
    void register_system(System<T_COMPONENT>* _system)
    {
        ASSERT(internal::s_currentSystemIndex < internal::cMaxRegisteredSystems, "Too many systems!  Increase cMaxRegisteredSystems!");

        internal::s_typeInfoKeys[internal::s_currentSystemIndex] = &typeid(T_COMPONENT);
        internal::s_systemValues[internal::s_currentSystemIndex] = (void*)_system;
        internal::s_currentSystemIndex++;
    }

    template <typename T_COMPONENT>
    System<T_COMPONENT>* get_system()
    {
        int index = -1;

        auto typeId = &typeid(T_COMPONENT);
        for (int i = 0; i < internal::s_currentSystemIndex; ++i) {
            if (internal::s_typeInfoKeys[i] == typeId) {
                index = i;
                break;
            }
        }

        if (index >= 0) {
            return reinterpret_cast<System<T_COMPONENT>*>(internal::s_systemValues[index]);
        }
        return nullptr;
    }

    inline EntityId create_entity()
    {
        return entity::create();
    }
    
    template <typename T_COMPONENT>
    ComponentId add_component(EntityId _entity)
    {
        auto system = get_system<T_COMPONENT>();
        return core_system::add_component(*system, _entity);
    }

    template <typename T_COMPONENT>
    ComponentHandle<T_COMPONENT> get_component(EntityId _entity)
    {
        auto system = get_system<T_COMPONENT>();
        return core_system::get_component(*system, _entity);
    }

    template <typename T_COMPONENT>
    ComponentHandle<T_COMPONENT> get_component_handle(ComponentId _component)
    {
        auto system = get_system<T_COMPONENT>();
        return core_system::get_component_handle(*system, _component);
    }


}