#include "material.h"
#include "hash.h"
#include "murmur_hash.h"

#include <glm/gtc/type_ptr.hpp>
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

        _materialOut._uniforms = new foundation::Hash<int>(foundation::memory_globals::default_allocator());

        return true;
    }

    void unload(Material& _material)
    {
        if (_material._programId > 0) {
            glDeleteProgram(_material._programId);
        }

        delete _material._uniforms;
    }

    void use(const Material& _material)
    {
        glUseProgram(_material._programId);
    }

    int load_uniform(Material& _material, const char* _uniformName)
    {
        uint64 key = foundation::murmur_hash_64(_uniformName, (uint32)strlen(_uniformName), 0);

        if (foundation::hash::has(*_material._uniforms, key)) {
            return foundation::hash::get(*_material._uniforms, key, 0);
        }

        int location = glGetUniformLocation(_material._programId, _uniformName);
        foundation::hash::set(*_material._uniforms, key, location);
        return location;
    }

    template <>
    void set_uniform<float32>(Material& _material, const char* _uniformName, const float32& _param)
    {
        int uniform = load_uniform(_material, _uniformName);
        glUniform1f(uniform, _param);
    }

    template <>
    void set_uniform<glm::mat4>(Material& _material, const char* _uniformName, const glm::mat4& _param)
    {
        int uniform = load_uniform(_material, _uniformName);
        glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(_param));
    }

    template <>
    void set_uniform<int>(Material& _material, const char* _uniformName, const int& _param)
    {
        int uniform = load_uniform(_material, _uniformName);
        glUniform1i(uniform, _param);
    }
}