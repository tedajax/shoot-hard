#pragma once

#include "types.h"
#include "shader.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>

namespace material
{
    bool create(ShaderProgram* _program, Material& _materialOut);
    void unload(Material& _material);

    void use(const Material& _material);

    template <typename T>
    void set_uniform(Material& _material, const char* _uniformName, const T& _param)
    {
        shader::set_uniform(*_material.program, _uniformName, _param);
    }
}