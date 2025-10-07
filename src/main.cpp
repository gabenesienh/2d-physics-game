// Platformer game

#include <SDL2/SDL.h>

#include "events.hpp"
#include "game.hpp"
#include "graphics.hpp"
#include "util.hpp"

double dt = 0;

// The number acquired from SDL_GetPerformanceCounter() the previous frame
// For calculating delta time
Uint64 ticksLast = 0;

int main(int argc, char** argv) {
    if (!init()) return 1;

    debugMode =   DEBUG_PERFORMANCE_INFO
                | DEBUG_PLAYER_INFO
                | DEBUG_SHOW_HITBOXES
                | DEBUG_SHOW_QUADS;

    while (true) {
        // Update delta time
        dt = SDL_GetPerformanceCounter() - ticksLast;
        dt *= 60.0/SDL_GetPerformanceFrequency();
        ticksLast = SDL_GetPerformanceCounter();

        if (!doEvents()) break;
        doGame();
        doRender();
    }

    kill();
    return 0;
}