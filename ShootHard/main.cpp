#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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
#include "camera.h"
#include "ref_counted.h"
#include "window.h"
#include "renderer.h"
#include "logger.h"

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
    logger::init();

    Window window;
    window::init(window, "Shoot Hard", 800, 600, WindowStyle::cWindowed);

    Renderer renderer;
    renderer::init(renderer, window, foundation::memory_globals::default_allocator());

    input::init();
    texture::manager::init(foundation::memory_globals::default_allocator(), renderer);

    Texture characterDiffuse = texture::get("Assets/p1_stand.png");

    bool isRunning = true;

    Camera camera;
    camera.position = glm::vec3(0.f, 0.f, 3.f);
    camera.projectionType = ProjectionType::cOrtho;
    camera.fov = 90.f;
    camera.aspectRatio = 4.f / 3.f;
    camera.nearZ = 0.1f;
    camera.farZ = 100.f;
    camera.orthoSize = 400.f;

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

        renderer::set_draw_color(renderer, color::create(0, 0, 0, 255));
        renderer::clear(renderer);

        renderer::set_draw_color(renderer, color::create(255, 255, 255, 255));
        renderer::copy(renderer, characterDiffuse, nullptr, nullptr);

        renderer::present(renderer);
    }


    texture::manager::terminate();
    input::terminate();
    renderer::shutdown(renderer);
    window::shutdown(window);

    logger::shutdown();

    return 0;
}

SDL_GLContext create_context(SDL_Window* _window, int _major, int _minor)
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, _major);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, _minor);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    return SDL_GL_CreateContext(_window);
}