#include "window.h"

#include <SDL2/SDL.h>

namespace window
{
    void init(Window& _window, const char* _title, int _width, int _height, WindowStyle _style)
    {
        ASSERT(_window._sdlWindow == nullptr, "Window has already been initialized.");

        _window._width = _width;
        _window._height = _height;
        _window._style = _style;

        uint32 flags = SDL_WINDOW_SHOWN;

        switch (_style) {
        default:
        case WindowStyle::cWindowed:
            break;

        case WindowStyle::cFullscreen:
            flags |= SDL_WINDOW_FULLSCREEN;
            break;

        case WindowStyle::cBorderlessWindow:
            flags |= SDL_WINDOW_BORDERLESS;
            flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
            break;
        }

        _window._sdlWindow = SDL_CreateWindow(
            _title,
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            _width, _height,
            flags
        );
    }

    void shutdown(Window& _window)
    {
        ASSERT(_window._sdlWindow != nullptr, "Cannot shutdown window as it has not been initialized.");

        SDL_DestroyWindow(_window._sdlWindow);
    }

    void on_resize(Window& _window, int _width, int _height)
    {
        _window._width = _width;
        _window._height = _height;
    }

    void resize(Window& _window, int _width, int _height)
    {
        SDL_SetWindowSize(_window._sdlWindow, _width, _height);
    }

    void set_style(Window& _window, WindowStyle _style)
    {
        ASSERT(_window._sdlWindow != nullptr, "");

        switch (_style) {
        case WindowStyle::cWindowed:
            SDL_SetWindowBordered(_window._sdlWindow, SDL_TRUE);
            SDL_SetWindowFullscreen(_window._sdlWindow, 0);
            break;

        case WindowStyle::cFullscreen:
            SDL_SetWindowFullscreen(_window._sdlWindow, SDL_WINDOW_FULLSCREEN);
            break;

        case WindowStyle::cBorderlessWindow:
            SDL_SetWindowBordered(_window._sdlWindow, SDL_FALSE);
            SDL_SetWindowFullscreen(_window._sdlWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
            break;
        }
    }

    void set_title(Window& _window, const char* _title)
    {
        ASSERT(_window._sdlWindow != nullptr, "");
        SDL_SetWindowTitle(_window._sdlWindow, _title);
    }
}
