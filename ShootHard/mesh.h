#pragma once

#include "types.h"

namespace mesh
{
    void set_vertex_data(Mesh& _mesh, uint32 _vertCount, const glm::vec3* _vertices, const glm::vec2* _uvs, const glm::vec3* _normals, const glm::vec4* _colors);
    void set_indices(Mesh& _mesh, const uint32* _indices, uint32 _count);

    MeshBuffers create_buffers(Mesh& _mesh);
}