#pragma once

#include "types.h"

namespace material
{
    bool create(Shader _vertexShader, Shader _fragmentShader, Material& _materialOut);
    void unload(Material& _material);
    void use(const Material& _material);
}