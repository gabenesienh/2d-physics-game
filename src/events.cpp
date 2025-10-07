#include "events.hpp"

#include <SDL2/SDL.h>
#include <array>

#include "util.hpp"

using std::array;

array<bool, SDL_NUM_SCANCODES> keyStates = {false};
array<bool, 5> mouseStates = {false};
vec2 mouseScreenPos;

SDL_Event event;

bool doEvents() {
    while (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
            case SDL_KEYDOWN:
                keyStates[event.key.keysym.scancode] = true;

                break;
            case SDL_KEYUP:
                keyStates[event.key.keysym.scancode] = false;

                break;
            case SDL_MOUSEMOTION:
                mouseScreenPos.x = event.motion.x;
                mouseScreenPos.y = event.motion.y;

                break;
            case SDL_MOUSEBUTTONDOWN:
                mouseStates[event.button.button] = true;

                break;
            case SDL_MOUSEBUTTONUP:
                mouseStates[event.button.button] = false;

                break;
            case SDL_QUIT:
                return false;
        }
    }
    
    return true;
}