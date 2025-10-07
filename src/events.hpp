// Input events (e.g. keyboard presses, mouse movement, window closing)

#ifndef EVENTS_HPP
#define EVENTS_HPP

#include <SDL2/SDL.h>
#include <array>

#include "util.hpp"

using std::array;

// The current state of all keyboard keys
// true means pressed, false means not pressed
extern array<bool, SDL_NUM_SCANCODES> keyStates;

// The current state of all mouse buttons
// true means pressed, false means not pressed
extern array<bool, 5> mouseStates;

// The current X and Y position of the mouse cursor on the screen
extern vec2 mouseScreenPos;

// Event loop
// Returns false when the window should close
extern bool doEvents();

#endif