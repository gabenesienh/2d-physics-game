#include "util.hpp"

#include <SDL2/SDL.h>
#include <iostream>

#include "game.hpp"
#include "objects.hpp"
#include "graphics.hpp"

using std::cin, std::cout, std::endl;

SDL_Window* window;
SDL_Surface* winSurface;
SDL_Surface* gameSurface;

/* -- AABBCommon class -- */

vec2<int> AABBCommon::intersects(AABBCommon& other) const {
    int intersectsX = INTERSECT_X_NONE;
    int intersectsY = INTERSECT_Y_NONE;

    // Check intersection in X axis
    if (this->getRightX() > other.getLeftX()
    &&  this->getLeftX()  < other.getRightX()) {
        bool touchingLeft = false;
        bool touchingRight = false;

        if (this->getLeftX()  < other.getLeftX()
        &&  this->getRightX() < other.getRightX()) {
            // This box's right side might be touching the other's left side
            touchingLeft = true;
        } else if (this->getLeftX()  > other.getLeftX()
          &&       this->getRightX() > other.getRightX()) {
            // This box's left side might be touching the other's right side
            touchingRight = true;
        }

        if (!touchingLeft && !touchingRight) {
            // This box contains/is contained by the other in the X axis
            intersectsX = INTERSECT_X_BOTH;
        } else {
            intersectsX = (touchingLeft) ? INTERSECT_X_LEFT : INTERSECT_X_RIGHT;
        }
    }

    if (intersectsX == INTERSECT_X_NONE) return INTERSECT_NONE;

    // Check intersection in Y axis
    if (this->getBottomY() > other.getTopY()
    &&  this->getTopY()    < other.getBottomY()) {
        bool touchingTop = false;
        bool touchingBottom = false;

        if (this->getTopY()    < other.getTopY()
        &&  this->getBottomY() < other.getBottomY()) {
            // This box's bottom side might be touching the other's top side
            touchingTop = true;
        } else if (this->getTopY()    > other.getTopY()
          &&       this->getBottomY() > other.getBottomY()) {
            // This box's top side might be touching the other's bottom side
            touchingBottom = true;
        }

        if (!touchingTop && !touchingBottom) {
            // This box contains/is contained by the other in the Y axis
            intersectsY = INTERSECT_Y_BOTH;
        } else {
            intersectsY = (touchingTop) ? INTERSECT_Y_TOP : INTERSECT_Y_BOTTOM;
        }
    }

    if (intersectsY == INTERSECT_Y_NONE) return INTERSECT_NONE;

    return {intersectsX, intersectsY};
}

AABBCommon::~AABBCommon() {};

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

    // Convert all colors to the window surface's format
    // Colors must initially be Uint32 formatted as RGB
    for (auto color = debugColors.begin(); color != debugColors.end(); color++) {
        int r = (color->second & 0xFF0000) >> 4*4;
        int g = (color->second & 0x00FF00) >> 4*2;
        int b = (color->second & 0x0000FF) >> 4*0;

        color->second = SDL_MapRGB(winSurface->format, r, g, b);
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