#pragma once

#include "types.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>

namespace material
{
    bool create(Shader _vertexShader, Shader _fragmentShader, Material& _materialOut);
    void unload(Material& _material);
    
    void use(const Material& _material);

    int load_uniform(Material& _material, const char* _uniformName);

    template <typename T>
    void set_uniform(Material& _material, const char* _uniformName, const T& _param)
    {
    }

    template <> void set_uniform<float32>(Material& _material, const char* _uniformName, const float32& _param);
    template <> void set_uniform<glm::mat4>(Material& _material, const char* _uniformName, const glm::mat4& _param);
    template <> void set_uniform<int>(Material& _material, const char* _uniformName, const int& _param);
}