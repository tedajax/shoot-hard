#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <memory>
#include <iostream>

#include "memory.h"
#include "types.h"
#include "array.h"
#include "sprite.h"

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
    SDL_Window* window = SDL_CreateWindow("Shoot Hard", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);

    foundation::Array<Sprite> sprites(foundation::memory_globals::default_allocator());
    foundation::array::set_capacity(sprites, 64);
    
    SDL_Surface* imgSurface = IMG_Load("../Assets/p1_stand.png");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, imgSurface);

    Sprite s;
    sprite::init(s, texture, 0);
    foundation::array::push_back(sprites, s);

    bool isRunning = true;

    while (isRunning) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                    isRunning = false;
                }
                break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderClear(renderer);

        sprite::render_sprites(renderer, sprites);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return 0;
}