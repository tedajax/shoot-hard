#include "material.h"

#include <iostream>
#include <gl/glew.h>

namespace material
{
    bool create(Shader _vertexShader, Shader _fragmentShader, Material& _materialOut)
    {
        if (_vertexShader._shaderType != ShaderType::cVertex ||
            _fragmentShader._shaderType != ShaderType::cFragment) {
            return false;
        }

        uint programId = glCreateProgram();
        glAttachShader(programId, _vertexShader._shaderId);
        glAttachShader(programId, _fragmentShader._shaderId);
        glLinkProgram(programId);

        int result, logLength;
        glGetProgramiv(programId, GL_LINK_STATUS, &result);
        glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);

        if (logLength > 0) {
            const int maxErrorLength = 256;
            char errorMessage[maxErrorLength];
            glGetProgramInfoLog(programId, logLength, nullptr, &errorMessage[0]);
            errorMessage[maxErrorLength - 1] = 0;
            std::cout << &errorMessage[0] << std::endl;
            glDeleteProgram(programId);
            return false;
        }

        _materialOut._programId = programId;

        return true;
    }

    void unload(Material& _material)
    {
        if (_material._programId > 0) {
            glDeleteProgram(_material._programId);
        }
    }

    void use(const Material& _material)
    {
        glUseProgram(_material._programId);
    }
}