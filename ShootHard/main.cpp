#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <gl/glew.h>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <iostream>

#include "memory.h"
#include "types.h"
#include "array.h"
#include "input.h"
#include "sprite.h"
#include "color.h"
#include "material.h"
#include "shader.h"
#include "mesh.h"

int run();
SDL_GLContext create_context(SDL_Window* _window, int _major, int _minor);

int main(int argc, char* argv[])
{
    foundation::memory_globals::init();

    int result = run();

    foundation::memory_globals::shutdown();

    return result;
}

int run()
{
    SDL_Window* window = SDL_CreateWindow("Shoot Hard", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);

    std::cout << "Creating GL context with version 4.2...\n";
    SDL_GLContext context = create_context(window, 4, 2);

    if (!context) {
        std::cout << "Failed, attempting to create context with version 3.2...\n";
        context = create_context(window, 3, 2);
    }

    if (!context) {
        std::cout << "SDL_GL_CreateContext() Error: " << SDL_GetError() << std::endl;
    }

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cout << "glewInit() Error: " << glewGetErrorString(err) << std::endl;
        std::cin.get();
        return 1;
    }

    uint vertexArrayId;
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    input::init();
    texture::manager::init(foundation::memory_globals::default_allocator(), renderer);

    Mesh cube;
    MeshBuffers cubeBuffers;
    {
        const int vertCount = 8;
        float32 halfSize = 0.5f;
        static const glm::vec3 vertexData[vertCount] = {
            { -halfSize, halfSize, halfSize },
            { -halfSize, -halfSize, halfSize },
            { halfSize, halfSize, halfSize },
            { halfSize, -halfSize, halfSize },
            { -halfSize, halfSize, -halfSize },
            { -halfSize, -halfSize, -halfSize },
            { halfSize, halfSize, -halfSize },
            { halfSize, -halfSize, -halfSize },
        };

        const int indexCount = 12 * 3;
        static const uint32 indexData[indexCount] = {
            0, 3, 1,
            0, 2, 3,

            3, 7, 2,
            6, 7, 2,

            6, 4, 5,
            6, 5, 7,

            5, 1, 4,
            4, 0, 1,

            0, 6, 2,
            0, 4, 6,

            1, 7, 3,
            1, 5, 7,
        };

        mesh::set_vertex_data(cube, vertCount, vertexData, nullptr, nullptr, nullptr);
        mesh::set_indices(cube, indexData, indexCount);

        cubeBuffers = mesh::create_buffers(cube);
    }

    Shader vertShader, fragShader;
    bool vertLoaded = shader::load("Assets/Shaders/basic.vert", ShaderType::cVertex, vertShader);
    bool fragLoaded = shader::load("Assets/Shaders/basic.frag", ShaderType::cFragment, fragShader);

    ASSERT(vertLoaded && fragLoaded, "");

    Material material;
    material::create(vertShader, fragShader, material);

    bool isRunning = true;

    float angle = 0.f;

    Mesh quad;
    mesh::create_quad(quad);
    MeshBuffers quadBuffers = mesh::create_buffers(quad);

    while (isRunning) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;

            case SDL_KEYDOWN:
                input::set_key_down(event.key.keysym.scancode);
                break;

            case SDL_KEYUP:
                input::set_key_up(event.key.keysym.scancode);
                break;
            }
        }

        if (input::get_key_down(SDL_SCANCODE_ESCAPE)) {
            isRunning = false;
        }

        glClearColor(0.1f, 0.1f, 0.1f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        material::use(material);

        angle += 0.01f;

        auto perspective = glm::perspective(glm::radians(90.f), 4.f / 3.f, 0.1f, 100.f);
        auto ortho = glm::ortho(-1.f, 1.f, -1.f, 1.f, 0.1f, 100.f);
        glm::mat4 projection = perspective;
        glm::mat4 view = glm::lookAt(glm::vec3(0.f, 0.f, 3.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
        glm::mat4 model = glm::rotate(glm::mat4(1.f), angle, glm::vec3(1.f, 0.1f, 0.f));

        auto mvp = projection * view * model;

        material::set_uniform<glm::mat4>(material, "MVP", mvp);

        MeshBuffers activeMesh = quadBuffers;

        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, activeMesh.vertexBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, activeMesh.indexBuffer);
        glDrawElements(GL_TRIANGLES, activeMesh.indexCount, GL_UNSIGNED_INT, (void*)0);

        glDisableVertexAttribArray(0);

        SDL_GL_SwapWindow(window);

        input::update();
    }

    material::unload(material);

    texture::manager::terminate();
    input::terminate();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return 0;
}

SDL_GLContext create_context(SDL_Window* _window, int _major, int _minor)
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    return SDL_GL_CreateContext(_window);
}