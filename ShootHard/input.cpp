#include "input.h"
#include "array.h"
#include <SDL2/SDL_scancode.h>

namespace input
{
    struct InputManager
    {
        bool _currentKeyStates[SDL_NUM_SCANCODES];
        bool _oldKeyStates[SDL_NUM_SCANCODES];
    };

    InputManager _inputManager;

    void init()
    {
        for (int i = 0; i < SDL_NUM_SCANCODES; ++i) {
            _inputManager._currentKeyStates[i] = false;
            _inputManager._oldKeyStates[i] = false;
        }
    }

    void terminate()
    {

    }

    void update()
    {
        for (int i = 0; i < SDL_NUM_SCANCODES; ++i) {
            _inputManager._oldKeyStates[i] = _inputManager._currentKeyStates[i];
        }
    }

    void set_key_down(int scancode)
    {
        _inputManager._currentKeyStates[scancode] = true;
    }

    void set_key_up(int scancode)
    {
        _inputManager._currentKeyStates[scancode] = false;
    }

    bool get_key(int scancode)
    {
        return _inputManager._currentKeyStates[scancode];
    }

    bool get_key_down(int scancode)
    {
        return _inputManager._currentKeyStates[scancode] && !_inputManager._oldKeyStates[scancode];
    }

    bool get_key_up(int scancode)
    {
        return !_inputManager._currentKeyStates[scancode] && _inputManager._oldKeyStates[scancode];
    }
}