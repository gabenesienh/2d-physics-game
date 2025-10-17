// SDL boilerplate and misc utilities

#ifndef UTIL_HPP
#define UTIL_HPP

#include <SDL2/SDL.h>

const int WINDOW_WIDTH = 960;
const int WINDOW_HEIGHT = 540;

extern SDL_Window* window;
extern SDL_Surface* winSurface;
extern SDL_Surface* gameSurface;

// Generic 2D vector
struct vec2 {
    double x;
    double y;

    bool operator==(const vec2& other) const;
    bool operator!=(const vec2& other) const;

    // Get a unit vector from this vec2
    vec2 normalized();
};

// Properties common to derived AABB types
struct AABBCommon {
    virtual double getTopY() const = 0;
    virtual double getBottomY() const = 0;
    virtual double getLeftX() const = 0;
    virtual double getRightX() const = 0;

    /*
     * Returns a vector of how much this box intersects the other. Values are
     * signed to represent direction in each axis. If not intersecting,
     * returns {0, 0}.
     *
     * To avoid virtual calls, this method does NOT consider the boxes' widths
     * and heights for the return value. This should be accounted for using
     * the derived classes' specific methods.
     */
    vec2 intersects(AABBCommon& other) const;

    virtual ~AABBCommon() = 0;
};

// Initialize SDL
extern bool init();

// Quit SDL2
extern void kill();

#endif