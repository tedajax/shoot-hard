#include "material.h"
#include "hash.h"
#include "murmur_hash.h"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <gl/glew.h>

namespace material
{
    bool create(ShaderProgram* _program, Material& _materialOut)
    {
        _materialOut.program = _program;
        return true;
    }

    void unload(Material& _material)
    {

    }

    void use(const Material& _material)
    {
        shader::use(*_material.program);
    }
}