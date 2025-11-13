#include "util.hpp"

#include <cmath>

using std::pow, std::sqrt;

/* -- vec2 class -- */

template <typename T>
bool vec2<T>::operator==(const vec2<T>& other) const {
    return (this->x == other.x && this->y == other.y);
}

template <typename T>
bool vec2<T>::operator!=(const vec2<T>& other) const {
    return !this->operator==(other);
}

template <typename T>
vec2<double> vec2<T>::normalized() {
    // Get vector magnitude (pythagorean theorem)
    double mag = sqrt(pow(this->x, 2) + pow(this->y, 2));

    return {
        this->x / mag,
        this->y / mag
    };
}