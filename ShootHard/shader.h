#pragma once

#include "types.h"

namespace shader
{
    bool load(const char* _filename, ShaderType _shaderType, Shader& _shaderOut);
    void unload(Shader& _shader);

    bool create_program(const Shader& _vertexShader, const Shader& _fragmentShader, ShaderProgram& _programOut);
    void unload_program(ShaderProgram& _program);

    void use(const ShaderProgram& _program);

    int load_uniform(ShaderProgram& _program, const char* _uniformName);

    template <typename T>
    void set_uniform(ShaderProgram& _program, const char* _uniformName, const T& _param)
    {
        ASSERT(false, "No set_uniform provided for given type T.");
    }

    template <> void set_uniform<int>(ShaderProgram& _program, const char* _uniformName, const int& _param);
    template <> void set_uniform<float32>(ShaderProgram& _program, const char* _uniformName, const float32& _param);
    template <> void set_uniform<glm::vec2>(ShaderProgram& _program, const char* _uniformName, const glm::vec2& _param);
    template <> void set_uniform<glm::vec3>(ShaderProgram& _program, const char* _uniformName, const glm::vec3& _param);
    template <> void set_uniform<glm::vec4>(ShaderProgram& _program, const char* _uniformName, const glm::vec4& _param);
    template <> void set_uniform<glm::mat4>(ShaderProgram& _program, const char* _uniformName, const glm::mat4& _param);

    class ProgramGuard
    {
    public:
        ProgramGuard(ShaderProgram& _program) : program(_program) { }
        ~ProgramGuard() { unload_program(program); }
    private:
        ShaderProgram program;
    };
}