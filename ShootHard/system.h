#pragma once

#include "types.h"
#include "component.h"
#include "memory.h"
#include <functional>

template <typename T_COMPONENT>
struct System
{
    static_assert(std::is_base_of<Component, T_COMPONENT>::value, "Template parameter T_COMPONENT must derive from Component.");

    typedef T_COMPONENT ComponentType;

    System(foundation::Allocator& _allocator)
        : _components(_allocator),
        _handles(nullptr),
        _allocator(_allocator)
    {
    }

    virtual ~System()
    {
        _allocator.deallocate(_handles);
    }

    foundation::Array<T_COMPONENT> _components;
    ComponentHandle<T_COMPONENT>* _handles;
    foundation::Allocator& _allocator;
    ComponentId _currentId;
};

namespace aspect_system
{
    template <typename T_COMPONENT>
    ComponentId add_component(System<T_COMPONENT>& _system, EntityId _entity)
    {
        T_COMPONENT component;
        component.entityId = _entity;
        component.componentId = get_next_component_id(_system);
        foundation::array::push_back(_system._components, component);
        return component.componentId;
    }

    template <typename T_COMPONENT>
    void add_handle(System<T_COMPONENT>& _system, ComponentHandle<T_COMPONENT>* _handle)
    {
        ASSERT(_handle->_next == nullptr, "Is this handle already added?");

        ComponentHandle<T_COMPONENT>* slider = _system._handles;

        if (slider == nullptr) {
            _system._handles = _handle;
        }
        else {
            while (slider->_next) {
                slider = slider->_next;
            }
            ASSERT(slider != _handle, "Double component handle added!");
            slider->_next = _handle;
        }
    }

    template <typename T_COMPONENT>
    void remove_handle(System<T_COMPONENT>& _system, ComponentHandle<T_COMPONENT>* _handle)
    {
        ASSERT(_handle != nullptr, "");

        ComponentHandle<T_COMPONENT>* slider = _system._handles;

        ASSERT(slider != nullptr, "Trying to remove handle from empty handle list.");

        if (slider == _handle) {
            _system._handles = _handle->_next;
        }
        else {
            while (slider->_next && slider->_next != _handle) {
                slider = slider->_next;
            }

            ASSERT(slider->_next != nullptr, "Failed to find handle in handle list.");

            slider->_next = _handle->next;
        }

        delete _handle;
    }

    template <typename T_COMPONENT>
    bool try_find_handle(System<T_COMPONENT>& _system, std::function<bool(ComponentHandle<T_COMPONENT>*)> predicate, ComponentHandle<T_COMPONENT>& _out)
    {
        auto* handle = _system._handles;
        while (handle) {
            if (predicate(handle)) {
                _out = *handle;
                return true;
            }
            handle = handle->_next;
        }
        return false;
    }

    template <typename T_COMPONENT>
    ComponentHandle<T_COMPONENT> get_component_on_entity(System<T_COMPONENT>& _system, EntityId _entity)
    {
        ComponentHandle<T_COMPONENT> find;
        auto predicate = [_entity](ComponentHandle<T_COMPONENT>* handle) -> bool {
            return handle->get()->entityId == _entity;
        };
        if (try_find_handle<T_COMPONENT>(_system, predicate, find)) {
            return find;
        }

        for (int i = 0, len = foundation::array::size(_system._components); i < len; ++i) {
            if (_system._components[i].entityId == _entity) {
                auto handle = component::create_handle(_system, _system._components[i].componentId);
                add_handle(_system, handle);
                return *handle;
            }
        }

        return component::create_empty_handle(_system);
    }

    template <typename T_COMPONENT>
    ComponentHandle<T_COMPONENT> get_component(System<T_COMPONENT>& _system, ComponentId _componentId)
    {
        ComponentHandle<T_COMPONENT> find;
        auto predicate = [=](ComponentHandle<T_COMPONENT>* handle) -> bool {
            return handle->_componentId == _componentId;
        };
        if (try_find_handle<T_COMPONENT>(_system, predicate, find)) {
            return find;
        }

        for (int i = 0, len = foundation::array::size(_system._components); i < len; ++i) {
            if (_system._components[i].componentId == _componentId) {
                auto handle = component::create_handle(_system, _system._components[i].componentId);
                add_handle(_system, handle);
                return *handle;
            }
        }

        return component::create_empty_handle(_system);
    }

    template <typename T_COMPONENT>
    void remove_component(System<T_COMPONENT>& _system, ComponentId _componentId)
    {

    }

    template <typename T_COMPONENT>
    ComponentId get_next_component_id(System<T_COMPONENT>& _system)
    {
        return _system._currentId++;
    }
}