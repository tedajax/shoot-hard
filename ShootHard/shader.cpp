#include "shader.h"
#include <string>
#include <fstream>
#include <iostream>
#include <gl/glew.h>

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
            std::cout << &errorMessage[0] << std::endl;
            return false;
        }

        Shader shader;
        _shaderOut._shaderId = shaderId;
        _shaderOut._shaderType = _shaderType;

        return true;
    }

    void unload(Shader& _shader)
    {
        if (_shader._shaderId > 0) {
            glDeleteShader(_shader._shaderId);
        }
    }
}