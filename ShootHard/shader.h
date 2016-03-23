#pragma once

#include "types.h"

namespace shader
{
    bool load(const char* _filename, ShaderType _shaderType, Shader& _shaderOut);
    void unload(Shader& _shader);
}