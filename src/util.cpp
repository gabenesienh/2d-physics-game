#include "util.hpp"

#include <SDL2/SDL.h>
#include <cmath>
#include <iostream>

#include "game.hpp"
#include "objects.hpp"

using std::pow, std::sqrt;
using std::cin, std::cout, std::endl;

SDL_Window* window;
SDL_Surface* winSurface;
SDL_Surface* gameSurface;

/* -- vec2 class -- */

bool vec2::operator==(const vec2& other) const {
    return (this->x == other.x && this->y == other.y);
}
bool vec2::operator!=(const vec2& other) const {
    return !this->operator==(other);
}

vec2 vec2::normalized() {
    // Get vector magnitude (pythagorean theorem)
    double mag = sqrt(pow(this->x, 2) + pow(this->y, 2));

    return {
        this->x /= mag,
        this->y /= mag
    };
}

/* -- BoundingBox class -- */

bool BoundingBox::intersects(BoundingBox& other) const {
    bool intersectsY = false;
    bool intersectsX = false;

    if (this->getBottomY() > other.getTopY()
    &&  this->getTopY()    < other.getBottomY()) {
        // This box might be intersecting the other from the top side
        intersectsY = true;
    } else if (this->getTopY()    < other.getBottomY()
      &&       this->getBottomY() > other.getTopY()   ) {
        // This box might be intersecting the other from the bottom side
        intersectsY = true;
    }

    // These boxes' top and bottom sides can't collide, so they can't intersect
    if (!intersectsY) return false;

    if (this->getRightX() > other.getLeftX()
    &&  this->getLeftX()  < other.getRightX()) {
        // This box might be intersecting the other from the left side
        intersectsX = true;
    } else if (this->getLeftX()  < other.getRightX()
      &&       this->getRightX() > other.getLeftX() ) {
        // This box might be intersecting the other from the right side
        intersectsX = true;
    }

    if (!intersectsX) return false;

    return true;
}

BoundingBox::~BoundingBox() {};

/* -- Utility methods -- */

bool init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        cout << "Error initializing SDL: " << SDL_GetError() << endl;
        system("pause");
        return false;
    }

    window = SDL_CreateWindow(
        "Game",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0
    );
    if (!window) {
        cout << "Error creating window: " << SDL_GetError() << endl;
        system("pause");
        return false;
    }

    winSurface = SDL_GetWindowSurface(window);
    if (!winSurface) {
        cout << "Error getting surface: " << SDL_GetError() << endl;
        system("pause");
        return false;
    }

    // Temporary surface, will be formatted into the game surface
    SDL_Surface* temp = SDL_CreateRGBSurfaceWithFormat(
        winSurface->flags,
        winSurface->w,
        winSurface->h,
        winSurface->format->BitsPerPixel,
        winSurface->format->format
    );
    if (!temp) {
        cout << "Error creating game surface: " << SDL_GetError() << endl;
        system("pause");
        return false;
    }

    // Convert game surface to window surface for faster blitting
    gameSurface = SDL_ConvertSurface(temp, winSurface->format, 0);
    if (!gameSurface) {
        cout << "Error formatting game surface: " << SDL_GetError() << endl;
        system("pause");
        return false;
    }

    return true;
}

void kill() {
    for (GameObject* gobj : gameObjects) {
        delete gobj;
    }

    gameObjectsTree->clear();
    delete(gameObjectsTree);

    SDL_DestroyWindow(window);
    SDL_Quit();
}