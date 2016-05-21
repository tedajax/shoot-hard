#pragma once

#include "types.h"

namespace window
{
    void init(Window& _window, const char* _title, int _width, int _height, WindowStyle _style);
    void shutdown(Window& _window);

	// should be called when intercepting window resize event.
    void on_resize(Window& _window, int _width, int _height);

	// force window to new size
    void resize(Window& _window, int _width, int _height);
    void set_style(Window& _window, WindowStyle _style);
    void set_title(Window& _window, const char* _title);
}