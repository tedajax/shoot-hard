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
#include "system.h"
#include "component_types.h"
#include "platform.h"

int run();
SDL_GLContext create_context(SDL_Window* _window, int _major, int _minor);

int main(int argc, char* argv[])
{
    core::init();
    int result = run();
    core::shutdown();

    return result;
}

int run()
{
    logger::init();

    Window window;
    window::init(window, "Shoot Hard", 1920, 1080, WindowStyle::cBorderlessWindow);

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

    int frames = 0;
    bool doFrames = false;

    System<TransformComponent> transformSystem(foundation::memory_globals::default_allocator());
    System<SpriteComponent> spriteSystem(foundation::memory_globals::default_allocator());

    EntityId entity = entity::create();
    auto xformId = aspect_system::add_component<TransformComponent>(transformSystem, entity);
    auto spriteId = aspect_system::add_component<SpriteComponent>(spriteSystem, entity);

    {
        auto transformHandle = aspect_system::get_component_on_entity(transformSystem, entity);
        auto xform = transformHandle.get();
        xform->transform.position.x = 5;

        auto spriteHandle = aspect_system::get_component_on_entity(spriteSystem, entity);
        spriteHandle.get()->sprite = character;
    }

    {
        auto transformHandle = aspect_system::get_component(transformSystem, xformId);
        transformHandle.get()->transform.position.y = 10;
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

        if (input::get_key_down(SDL_SCANCODE_SPACE)) {
            doFrames = true;
        }

        renderer::set_draw_color(renderer, color::create(0, 0, 0, 255));
        renderer::clear(renderer);

        renderer::set_draw_color(renderer, color::create(255, 255, 255, 255));

        for (int i = 0; i < 500; ++i) {
            Transform t = transform::identity();
            t.position.x = i * 8.f + sin((float32)frames / (240 + i)) * 240;
            t.position.y = i * 4.5f + cos((float32)frames / (240 + i)) * 240;
            t.scale.x += i * 0.01f;
            character.layer = i;
            character.color.r = frames / 60 + i;
            character.color.g = frames / 120 + i;
            character.color.b = frames / 240 + i;
            renderer::add_draw_call(renderer, RenderBuckets::cGameLayer, character, t);
        }

        renderer::render(renderer);
        if (doFrames) ++frames;
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