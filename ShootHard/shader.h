#pragma once

#include "types.h"

namespace shader
{
    Shader load(const char* _filename, ShaderType _shaderType);
    void unload(Shader& _shader);
}