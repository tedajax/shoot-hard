#pragma once

#include <memory>
#include "basic_types.h"

namespace resource
{
    typedef uint64 ResourceId;
    const static uint64 cInvalidResourceId = 0xF457F00DDEADBEAF;

    struct Resource
    {
        ResourceId _resourceId;
    };

    template <typename T, int N>
    class ResourceManager
    {
        static_assert(std::is_base_of<Resource, T>::value, "ResourceManager templated type must be subclass of Resource.");

    public:
        ResourceManager();
        ~ResourceManager();

        bool load(const char* _filename, ResourceId& _idOut);
        void unload(const ResourceId& _id);

        T* get(const ResourceId& _id);

    private:
        int pop_index();
        int peek_index();
        void push_index(int index);

    private:
        T resources[N];
        int freeStack[N];
        int stackHead;
        ResourceId currentId;
    };

    template <typename T, int N>
    resource::ResourceManager<T, N>::ResourceManager()
    {
        for (int i = N - 1; i >= 0; --i) {
            auto resource = &resources[i];
            resource->_resourceId = cInvalidResourceId;
            push_index(i);
        }
        stackHead = N - 1;
        currentId = 1;
    }

    template <typename T, int N>
    resource::ResourceManager<T, N>::~ResourceManager()
    {
        for (int i = 0; i < N; ++i) {
            auto id = resources[i]._resourceId;
            if (id != cInvalidResourceId) {
                unload(id);
            }
        }
    }

    template <typename T, int N>
    bool resource::ResourceManager<T, N>::load(const char* _filename, ResourceId& _idOut)
    {
        int index = pop_index();

        auto resource = new (&resources[index]) T();
        
        if (resource->load(_filename)) {
            resource->_resourceId = currentId;
            ++currentId;
            return resource;
        }
        else {
            ASSERTF_ALWAYS(false, "Failed to load resource: \'%s\'", _filename);
        }
    }

    template <typename T, int N>
    void resource::ResourceManager<T, N>::unload(const ResourceId& _id)
    {
        for (int i = 0; i < N; ++i) {
            auto resource = &resources[i];
            if (resource->_resourceId == _id) {
                resource->unload(_filename);
                resource->~T();
                resource->_id = cInvalidResourceId;
                push_index(i);
                return;
            }
        }
    }

    template <typename T, int N>
    T* resource::ResourceManager<T, N>::get(const ResourceId& _id)
    {
        for (int i = 0; i < N; ++i) {
            auto resource = &resources[i];
            if (resource->_resourceId == _id) {
                return resource;
            }
        }
        return nullptr;
    }

    template <typename T, int N>
    int resource::ResourceManager<T, N>::pop_index()
    {
        ASSERT(stackHead > 0, "No more free space in manager!");
        stackHead--;
        return freeStack[stackHead];
    }

    template <typename T, int N>
    int resource::ResourceManager<T, N>::peek_index()
    {
        ASSERT(stackHead > 0, "No more free space in manager!");
        return freeStack[stackHead - 1];
    }

    template <typename T, int N>
    void resource::ResourceManager<T, N>::push_index(int index)
    {
        ASSERT(stackHead < N, "No more space available in stack!");
        freeStack[stackHead] = index;
        ++stackHead;
    }

}