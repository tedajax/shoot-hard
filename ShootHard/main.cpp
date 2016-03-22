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

int run();

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
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GLContext context = SDL_GL_CreateContext(window);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
        std::cin.get();
        return 1;
    }

    uint vertexArrayId;
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    input::init();
    texture::manager::init(foundation::memory_globals::default_allocator(), renderer);

    static const float32 vertexData[] = {
        -1.f, -1.f, 0.f,
        1.f, -1.f, 0.f,
        0.f, 1.f, 0.f,
    };

    uint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    auto vertShader = shader::load("Assets/Shaders/basic.vert", ShaderType::cVertex);
    auto fragShader = shader::load("Assets/Shaders/basic.frag", ShaderType::cFragment);
    Material material;
    material::create(vertShader, fragShader, material);

    bool isRunning = true;

    float angle = 0.f;

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

        angle += 0.01f;

        glm::mat4 projection = glm::perspective(glm::radians(90.f), 4.f / 3.f, 0.1f, 100.f);
        glm::mat4 view = glm::lookAt(glm::vec3(0.f, 0.f, -10.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
        glm::mat4 model = glm::rotate(glm::mat4(1.f), angle, glm::vec3(0.f, 1.f, 0.f));

        material::set_uniform<glm::mat4>(material, "MVP", projection * view * model);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glDrawArrays(GL_TRIANGLES, 0, 3);
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