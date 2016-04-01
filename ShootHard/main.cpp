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
#include "camera.h"

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

    input::init();
    texture::manager::init(foundation::memory_globals::default_allocator());

    Texture characterTexture = texture::get("Assets/p1_stand.png");

    Shader vertShader, fragShader;
    bool vertLoaded = shader::load("Assets/Shaders/basic.vert", ShaderType::cVertex, vertShader);
    bool fragLoaded = shader::load("Assets/Shaders/basic.frag", ShaderType::cFragment, fragShader);

    ASSERT(vertLoaded && fragLoaded, "");

    Material material;
    material::create(vertShader, fragShader, material);

    bool isRunning = true;

    float angle = 0.f;

    Mesh quadMesh;
    mesh::create_quad(quadMesh);
    MeshInstance quad = mesh::create_instance(quadMesh);

    Camera camera;
    camera.position = glm::vec3(0.f, 0.f, 3.f);
    camera.projectionType = ProjectionType::cOrtho;
    camera.fov = 90.f;
    camera.aspectRatio = 4.f / 3.f;
    camera.nearZ = 0.1f;
    camera.farZ = 100.f;
    camera.orthoSize = 400.f;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

        material::use(material);

        angle += 0.1f;
        glm::mat4 model;
        math::matrix::trs(glm::vec3(angle, 0.f, 0.f), glm::rotate(glm::quat(), angle, glm::vec3(0.f, 0.f, -1.f)), glm::vec3(68.f, 92.f, 1.f), model);

        auto viewProjection = camera::view_projection(camera);

        material::set_uniform<glm::mat4>(material, "mxViewProjection", viewProjection);
        material::set_uniform<glm::mat4>(material, "mxModel", model);

        MeshInstance activeMesh = quad;

        glActiveTexture(GL_TEXTURE0);
        texture::bind(characterTexture);
        material::set_uniform<int>(material, "mainTexture", 0);

        mesh::bind(activeMesh);

        mesh::render(activeMesh);

        material::set_uniform<glm::mat4>(material, "mxModel", math::matrix::trs(glm::vec3(100.f, 0.f, 0.f), glm::quat(), glm::vec3(68.f, 92.f, 1.f)));
        mesh::render(activeMesh);

        mesh::unbind(activeMesh);

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