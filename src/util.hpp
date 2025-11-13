// SDL boilerplate and misc utilities

#ifndef UTIL_HPP
#define UTIL_HPP

#include <SDL2/SDL.h>

const int WINDOW_WIDTH  = 960;
const int WINDOW_HEIGHT = 540;

const int INTERSECT_X_NONE  = 0;
const int INTERSECT_X_LEFT  = 1;
const int INTERSECT_X_RIGHT = 2;
const int INTERSECT_X_BOTH  = 3;

const int INTERSECT_Y_NONE   = 0;
const int INTERSECT_Y_TOP    = 1;
const int INTERSECT_Y_BOTTOM = 2;
const int INTERSECT_Y_BOTH   = 3;

extern SDL_Window* window;
extern SDL_Surface* winSurface;
extern SDL_Surface* gameSurface;

// Generic 2D vector
template <typename T>
struct vec2 {
    T x;
    T y;

    bool operator==(const vec2<T>& other) const;
    bool operator!=(const vec2<T>& other) const;

    // Get a unit vector from this vec2
    vec2<double> normalized();
};

#include "util.tpp"

const vec2<int> INTERSECT_NONE = {INTERSECT_X_NONE, INTERSECT_Y_NONE};

// Properties common to derived AABB types
struct AABBCommon {
    virtual double getTopY() const = 0;
    virtual double getBottomY() const = 0;
    virtual double getLeftX() const = 0;
    virtual double getRightX() const = 0;

    /* 
     * Returns the direction from which this box intersects the other
     * 
     * Will return INTERSECT_NONE if there isn't an intersection on both axes
     * 
     * Will return INTERSECT_(X/Y)_BOTH for a coordinate if either box fully
     * contains the other in that coordinate
     */
    vec2<int> intersects(AABBCommon& other) const;

    virtual ~AABBCommon() = 0;
};

// Initialize SDL
extern bool init();

// Quit SDL2
extern void kill();

#endif