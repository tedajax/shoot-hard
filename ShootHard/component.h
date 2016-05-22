#pragma once

#include "types.h"
#include "entity.h"

typedef uint64 ComponentId;

static const ComponentId cInvalidComponentId = 0xFFFFFFFFFFFFFFFF;

struct Component
{
    EntityId entityId;
    ComponentId componentId;
};

template <typename T_COMPONENT>
struct System;

template <typename T_COMPONENT>
struct ComponentHandle
{
    T_COMPONENT* get()
    {
        if (_componentId == cInvalidComponentId) {
            return nullptr;
        }

        if (_cachedValue == nullptr) {
            for (int i = 0, len = foundation::array::size(_system->_components); i < len; ++i) {
                if (_system->_components[i].componentId == _componentId) {
                    _cachedValue = &_system->_components[i];
                    break;
                }
            }
        }

        ASSERT(_cachedValue != nullptr, "Unable to find component in system.  Handle is out of date.");

        return _cachedValue;
    }

    System<T_COMPONENT>* _system = nullptr;
    ComponentId _componentId = cInvalidComponentId;
    ComponentHandle<T_COMPONENT>* _next = nullptr;
    T_COMPONENT* _cachedValue = nullptr;
};

namespace component
{
    template <typename T_COMPONENT>
    ComponentHandle<T_COMPONENT> create_empty_handle(System<T_COMPONENT>& _system)
    {
        ComponentHandle<T_COMPONENT> result;
        result._system = &_system;
        result._componentId = cInvalidComponentId;
        return result;
    }

    template <typename T_COMPONENT>
    ComponentHandle<T_COMPONENT>* create_handle(System<T_COMPONENT>& _system, ComponentId _componentId)
    {
        void* buffer = _system._allocator.allocate(sizeof(ComponentHandle<T_COMPONENT>));
        auto* result = new (buffer) ComponentHandle<T_COMPONENT>();

        result->_system = &_system;
        result->_componentId = _componentId;

        return result;
    }
}