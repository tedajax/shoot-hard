#pragma once

#include "types.h"

namespace mesh
{
    void set_vertex_data(Mesh& _mesh, uint32 _vertCount, const glm::vec3* _vertices, const glm::vec2* _uvs, const glm::vec3* _normals, const glm::vec4* _colors);
    void set_indices(Mesh& _mesh, const uint32* _indices, uint32 _count);

    void create_quad(Mesh& _meshOut, float32 _width = 1.f, float32 _height = 1.f);
    void create_quad(Mesh& _meshOut, const Rectangle& _uvRect, float32 _width = 1.f, float32 _height = 1.f);

    MeshInstance create_instance(Mesh& _mesh);

    void bind(const MeshInstance& _mesh);
    void unbind(const MeshInstance& _mesh);
    void render(const MeshInstance& _mesh);
}