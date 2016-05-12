#include <string>
#include <fstream>
#include <iostream>
#include <gl/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "hash.h"
#include "murmur_hash.h"

#include "shader.h"

namespace shader
{
    bool load(const char* _filename, ShaderType _shaderType, Shader& _shaderOut)
    {
        auto glType = (_shaderType == ShaderType::cVertex) ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
        uint shaderId = glCreateShader(glType);

        std::string shaderCode;

        {
            std::ifstream fileStream(_filename, std::ios::in);
            if (!fileStream.is_open()) {
                std::cout << "Unable to open " << _filename << std::endl;
                return false;
            }
            fileStream.seekg(0, std::ios::end);
            shaderCode.reserve(fileStream.tellg());
            fileStream.seekg(0, std::ios::beg);
            shaderCode.assign(std::istreambuf_iterator<char>(fileStream), std::istreambuf_iterator<char>());
        }

        auto sourcePtr = shaderCode.c_str();
        glShaderSource(shaderId, 1, &sourcePtr, nullptr);
        glCompileShader(shaderId);

        int result, logLength;
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);

        if (logLength > 0) {
            const int maxErrorLength = 256;
            char errorMessage[maxErrorLength];
            glGetShaderInfoLog(shaderId, logLength, nullptr, &errorMessage[0]);
            errorMessage[maxErrorLength - 1] = 0;
            std::cout << "Shader compile error: " << _filename << std::endl;
            std::cout << &errorMessage[0] << std::endl;
            return false;
        }

        Shader shader;
        _shaderOut.shaderId = shaderId;
        _shaderOut.shaderType = _shaderType;

        return true;
    }

    void unload(Shader& _shader)
    {
        if (_shader.shaderId > 0) {
            glDeleteShader(_shader.shaderId);
        }
    }

    bool create_program(const Shader& _vertexShader, const Shader& _fragmentShader, ShaderProgram& _programOut)
    {
        if (_vertexShader.shaderType != ShaderType::cVertex ||
            _fragmentShader.shaderType != ShaderType::cFragment) {
            return false;
        }

        uint programId = glCreateProgram();
        glAttachShader(programId, _vertexShader.shaderId);
        glAttachShader(programId, _fragmentShader.shaderId);
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

        _programOut.programId = programId;
        _programOut.uniforms = new foundation::Hash<int>(foundation::memory_globals::default_allocator());

        return true;
    }

    void unload_program(ShaderProgram& _program)
    {
        if (_program.programId > 0) {
            glDeleteProgram(_program.programId);
        }

        delete _program.uniforms;
    }

    void use(const ShaderProgram& _program)
    {
        glUseProgram(_program.programId);
    }

    int load_uniform(ShaderProgram& _program, const char* _uniformName)
    {
        uint64 key = foundation::murmur_hash_64(_uniformName, (uint32)strlen(_uniformName), 0);

        if (foundation::hash::has(*_program.uniforms, key)) {
            return foundation::hash::get(*_program.uniforms, key, 0);
        }

        int location = glGetUniformLocation(_program.programId, _uniformName);

        ASSERTF(location >= 0, "Invalid uniform %s", _uniformName);

        foundation::hash::set(*_program.uniforms, key, location);
        return location;
    }

    template <>
    void set_uniform<int>(ShaderProgram& _program, const char* _uniformName, const int& _param)
    {
        int uniform = load_uniform(_program, _uniformName);
        glUniform1i(uniform, _param);
    }

    template <>
    void set_uniform<float32>(ShaderProgram& _program, const char* _uniformName, const float32& _param)
    {
        int uniform = load_uniform(_program, _uniformName);
        glUniform1f(uniform, _param);
    }

    template <>
    void set_uniform<glm::vec2>(ShaderProgram& _program, const char* _uniformName, const glm::vec2& _param)
    {
        int uniform = load_uniform(_program, _uniformName);
        glUniform2fv(uniform, 1, glm::value_ptr(_param));
    }

    template <>
    void set_uniform<glm::vec3>(ShaderProgram& _program, const char* _uniformName, const glm::vec3& _param)
    {
        int uniform = load_uniform(_program, _uniformName);
        glUniform3fv(uniform, 1, glm::value_ptr(_param));
    }

    template <>
    void set_uniform<glm::vec4>(ShaderProgram& _program, const char* _uniformName, const glm::vec4& _param)
    {
        int uniform = load_uniform(_program, _uniformName);
        glUniform4fv(uniform, 1, glm::value_ptr(_param));
    }

    template <>
    void set_uniform<glm::mat4>(ShaderProgram& _program, const char* _uniformName, const glm::mat4& _param)
    {
        int uniform = load_uniform(_program, _uniformName);
        glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(_param));
    }
}