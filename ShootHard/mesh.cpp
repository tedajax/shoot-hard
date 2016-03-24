#include "mesh.h"
#include "array.h"
#include "memory.h"
#include "rectangle.h"
#include <new>
#include <cassert>
#include <gl/glew.h>

namespace mesh
{
    void set_vertex_data(Mesh& _mesh, uint32 _vertCount, const glm::vec3* _vertices, const glm::vec2* _uvs, const glm::vec3* _normals, const glm::vec4* _colors)
    {
        ASSERT(_vertices != nullptr, "Vertices must be supplied.");
        ASSERT(_vertCount > 0, "Invalid vertex count.");

        foundation::array::copy_in(*_mesh.vertices, _vertices, _vertCount);

        if (_uvs) {
            foundation::array::copy_in(*_mesh.uvs, _uvs, _vertCount);
        }

        if (_normals) {
            foundation::array::copy_in(*_mesh.normals, _normals, _vertCount);
        }

        if (_colors) {
            foundation::array::copy_in(*_mesh.colors, _colors, _vertCount);
        }
    }

    void set_indices(Mesh& _mesh, const uint32* _indices, uint32 _count)
    {
        ASSERT(_indices != nullptr, "Indices must be supplied.");

        foundation::array::copy_in(*_mesh.indices, _indices, _count);
    }

    void create_quad(Mesh& _meshOut, float32 _width /* = 1.f */, float32 _height /* = 1.f */)
    {
        return create_quad(_meshOut, rectangle::default_uvs(), _width, _height);
    }

    void create_quad(Mesh& _meshOut, const Rectangle& _uvRect, float32 _width /* = 1.f */, float32 _height /* = 1.f =*/)
    {
        float32 hw = _width / 2.f;
        float32 hh = _height / 2.f;

        static const glm::vec3 vertices[4] = {
            { -hw, hh, 0.f },
            { hw, hh, 0.f },
            { -hw, -hh, 0.f },
            { hw, -hh, 0.f },
        };

        const glm::vec2 uvs[4] = {
            { _uvRect.position.x, _uvRect.position.y },
            { _uvRect.position.x + _uvRect.width, _uvRect.position.y },
            { _uvRect.position.x, _uvRect.position.y + _uvRect.height },
            { _uvRect.position.x + _uvRect.width, _uvRect.position.y + _uvRect.height },
        };

        static const uint32 indices[6] = {
            0, 3, 2,
            1, 3, 0,
        };

        set_vertex_data(_meshOut, 4, vertices, uvs, nullptr, nullptr);
        set_indices(_meshOut, indices, 6);
    }

    MeshBuffers create_buffers(Mesh& _mesh)
    {
        MeshBuffers buffers;

        if (foundation::array::size(*_mesh.vertices) > 0) {
            glGenBuffers(1, &buffers.vertexBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, buffers.vertexBuffer);
            glBufferData(GL_ARRAY_BUFFER, foundation::array::size(*_mesh.vertices) * sizeof(glm::vec3), &(*_mesh.vertices)[0].x, GL_STATIC_DRAW);
        }

        if (foundation::array::size(*_mesh.normals) > 0) {
            glGenBuffers(1, &buffers.normalBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, buffers.normalBuffer);
            glBufferData(GL_ARRAY_BUFFER, foundation::array::size(*_mesh.normals) * sizeof(glm::vec3), &(*_mesh.normals)[0].x, GL_STATIC_DRAW);
        }

        if (foundation::array::size(*_mesh.uvs) > 0) {
            glGenBuffers(1, &buffers.uvBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, buffers.uvBuffer);
            glBufferData(GL_ARRAY_BUFFER, foundation::array::size(*_mesh.uvs) * sizeof(glm::vec2), &(*_mesh.uvs)[0].x, GL_STATIC_DRAW);
        }

        if (foundation::array::size(*_mesh.colors) > 0) {
            glGenBuffers(1, &buffers.colorBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, buffers.colorBuffer);
            glBufferData(GL_ARRAY_BUFFER, foundation::array::size(*_mesh.colors) * sizeof(glm::vec4), &(*_mesh.colors)[0].x, GL_STATIC_DRAW);
        }

        buffers.indexCount = foundation::array::size(*_mesh.indices);

        if (buffers.indexCount > 0) {
            glGenBuffers(1, &buffers.indexBuffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers.indexBuffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, foundation::array::size(*_mesh.indices) * sizeof(uint32), &(*_mesh.indices)[0], GL_STATIC_DRAW);
        }

        return buffers;
    }
}

Mesh::Mesh()
    : Mesh(foundation::memory_globals::default_allocator())
{ }

Mesh::Mesh(foundation::Allocator& _a)
{
    byte* ptr = _buffer;

    vertices = new (ptr) foundation::Array<glm::vec3>(_a);
    ptr += sizeof(foundation::Array<glm::vec3>);
    normals = new (ptr) foundation::Array<glm::vec3>(_a);
    ptr += sizeof(foundation::Array<glm::vec3>);
    uvs = new (ptr) foundation::Array<glm::vec2>(_a);
    ptr += sizeof(foundation::Array<glm::vec2>);
    colors = new (ptr) foundation::Array<glm::vec4>(_a);
    ptr += sizeof(foundation::Array<glm::vec4>);
    indices = new (ptr) foundation::Array<uint32>(_a);
}

Mesh::~Mesh()
{
    vertices->~Array();
    normals->~Array();
    uvs->~Array();
    colors->~Array();
    indices->~Array();
}