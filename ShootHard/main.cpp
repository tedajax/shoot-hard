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
#include "transform.h"
#include "platform.h"
#include "component_entity.h"

int run();
SDL_GLContext create_context(SDL_Window* _window, int _major, int _minor);

int main(int argc, char* argv[])
{
    logger::init();
    core::init();
    logger::debug("Handle", "%d", sizeof(ComponentHandle<TransformComponent>));
    int result = run();
    core::shutdown();
    logger::shutdown();

    return result;
}

int run()
{

    Window window;
    window::init(window, "Shoot Hard", 1280, 720, WindowStyle::cWindowed);

    Renderer renderer;
    renderer::init(renderer, window, foundation::memory_globals::default_allocator());

    input::init();
    texture::manager::init(foundation::memory_globals::default_allocator(), renderer);

    Texture characterDiffuse = texture::get("Assets/p1_stand.png");
    Sprite character;
    sprite::init(character, characterDiffuse);
    character.color = color::cWhite;

    bool isRunning = true;

    Camera camera;
    camera.position = glm::vec3(0.f, 0.f, 3.f);
    camera.projectionType = ProjectionType::cOrtho;
    camera.fov = 90.f;
    camera.aspectRatio = 4.f / 3.f;
    camera.nearZ = 0.1f;
    camera.farZ = 100.f;
    camera.orthoSize = 400.f;

    System<TransformComponent> transformSystem(foundation::memory_globals::default_allocator());
    System<SpriteComponent> spriteSystem(foundation::memory_globals::default_allocator());

    component_entity::register_system(&transformSystem);
    component_entity::register_system(&spriteSystem);

    EntityId entity = entity::create();

    for (int i = 0; i < 10; ++i) {
        EntityId entity = entity::create();

        auto xformId = component_entity::add_component<TransformComponent>(entity);
        auto spriteId = component_entity::add_component<SpriteComponent>(entity);

        auto xformHandle = component_entity::get_component_handle<TransformComponent>(xformId);
        auto spriteHandle = component_entity::get_component_handle<SpriteComponent>(spriteId);

        xformHandle.get()->transform.position.x = i * 16;
        xformHandle.get()->transform.position.y = i * 9;

        spriteHandle.get()->sprite = character;
    }

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

        sprite_system::render(spriteSystem, renderer);

        renderer::render(renderer);
    }


    texture::manager::terminate();
    input::terminate();
    renderer::shutdown(renderer);
    window::shutdown(window);

    return 0;
}

SDL_GLContext create_context(SDL_Window* _window, int _major, int _minor)
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, _major);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, _minor);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    return SDL_GL_CreateContext(_window);
}