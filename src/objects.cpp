//TODO: validate position on GameObject::tryMove
//TODO: validate screen position getters for GameObject
//TODO: validate GameObject::isVisible

#include "objects.hpp"

#include <cmath>
#include <string>
#include <stdexcept>

#include "util.hpp"

using std::abs;
using std::string;

/* -- AABB -- */

// Constructors
AABB::AABB(GameObject* parent, vec2 center, double halfWidth, double halfHeight)
    : parent(parent),
      center(center),
      halfWidth(halfWidth),
      halfHeight(halfHeight) {}

// Getters
double AABB::getTopY() const    { return this->center.y - this->halfHeight; }
double AABB::getBottomY() const { return this->center.y + this->halfHeight; }
double AABB::getLeftX() const   { return this->center.x - this->halfWidth; }
double AABB::getRightX() const  { return this->center.x + this->halfWidth; }

/* -- GameObject -- */

/* IMPORTANT: For most cases, even internally, you do not want to manipulate
 * this->bounds.center, since it refers to the bounding box's center,
 * without accounting for the object's alignment anchor! Instead, use the
 * proper getters and setters
 */

// Getters
AABB&     GameObject::getBounds()              { return this->bounds; }
eAnchorX  GameObject::getAnchorOffsetX() const { return this->anchorOffsetX; }
eAnchorY  GameObject::getAnchorOffsetY() const { return this->anchorOffsetY; }
double    GameObject::getSpeedX() const        { return this->speedX; }
double    GameObject::getSpeedY() const        { return this->speedY; }
double    GameObject::getMoveSpeed() const     { return this->moveSpeed; }
string    GameObject::getState() const         { return this->state; }
vec2      GameObject::getDirection() const     { return this->direction; }
eDirTypes GameObject::getDirectionType() const { return this->directionType; }
double    GameObject::getWeight() const        { return this->weight; }
vec2      GameObject::getAimDirection() const  { return this->aimDirection; }
eAnchorX  GameObject::getAimOffsetX() const    { return this->aimOffsetX; }
eAnchorY  GameObject::getAimOffsetY() const    { return this->aimOffsetY; }

double GameObject::getX() const {
    switch (this->anchorOffsetX) {
        case eAnchorX::left:
            return this->bounds.center.x - this->bounds.halfWidth;
        case eAnchorX::middle:
            return this->bounds.center.x;
        case eAnchorX::right:
            return this->bounds.center.x + this->bounds.halfWidth;
        default:
            throw std::logic_error("An object's anchorOffsetX is invalid (somehow).");
    }
}
double GameObject::getY() const {
    switch (this->anchorOffsetY) {
        case eAnchorY::top:
            return this->bounds.center.y - this->bounds.halfHeight;
        case eAnchorY::middle:
            return this->bounds.center.y;
        case eAnchorY::bottom:
            return this->bounds.center.y + this->bounds.halfHeight;
        default:
            throw std::logic_error("An object's anchorOffsetY is invalid (somehow).");
    }
}
double GameObject::getWidth() const {
    return this->bounds.halfWidth*2;
}
double GameObject::getHeight() const {
    return this->bounds.halfHeight*2;
}
double GameObject::getScreenX() const {
    return this->getX();
}
double GameObject::getScreenY() const {
    return this->getY();
}

// Setters
void GameObject::setWidth(double width) {
    this->bounds.halfWidth = width/2;
}
void GameObject::setHeight(double height) {
    this->bounds.halfHeight = height/2;
}
void GameObject::setSpeedX(double speedX) {
    this->speedX = speedX;
}
void GameObject::setSpeedY(double speedY) {
    this->speedY = speedY;
}
void GameObject::setMoveSpeed(double moveSpeed) {
    this->moveSpeed = moveSpeed;
}
void GameObject::setState(string state) {
    this->state = state;
}
bool GameObject::setDirection(vec2 direction) {
    // Normalize desired direction into unit vector
    direction = direction.normalized();

    // Ensure that the desired direction matches the object's direction type
    switch (this->directionType) {
        case eDirTypes::none:
            if (direction != DIR_NONE) {
                return false;
            }
            break;
        case eDirTypes::horizontal:
            if (direction != DIR_LEFT
            &&  direction != DIR_RIGHT) {
                return false;
            }
            break;
        case eDirTypes::orthogonal:
            if (direction != DIR_LEFT
            &&  direction != DIR_RIGHT
            &&  direction != DIR_UP
            &&  direction != DIR_DOWN) {
                return false;
            }
            break;
        case eDirTypes::omni:
            break;
    }

    this->direction = direction;
    return true;
}
void GameObject::setWeight(double weight) {
    this->weight = weight;
}

// Other methods
bool GameObject::isVisible() const {
    return true;
}
void GameObject::teleport(double x, double y) {
    double destX = this->bounds.center.x;
    double destY = this->bounds.center.y;

    switch (this->anchorOffsetX) {
        case eAnchorX::left:
            destX = x + this->bounds.halfWidth;
            break;
        case eAnchorX::middle:
            destX = x;
            break;
        case eAnchorX::right:
            destX = x - this->bounds.halfWidth;
            break;
    }
    switch (this->anchorOffsetY) {
        case eAnchorY::top:
            destY = y + this->bounds.halfHeight;
            break;
        case eAnchorY::middle:
            destY = y;
            break;
        case eAnchorY::bottom:
            destY = y - this->bounds.halfHeight;
            break;
    }

    this->bounds.center.x = destX;
    this->bounds.center.y = destY;
}
void GameObject::thrust(double addX, double addY) {
    this->speedX += addX;
    this->speedY += addY;
}
bool GameObject::tryMove(double x, double y) {
    this->teleport(x, y);

    return true;
}
void GameObject::walk() {
    this->speedX = this->direction.x * this->moveSpeed;
    this->speedY = this->direction.y * this->moveSpeed;
}
void GameObject::walk(vec2 direction) {
    direction = direction.normalized();

    this->speedX = direction.x * this->moveSpeed;
    this->speedY = direction.y * this->moveSpeed;
}
void GameObject::aimAt(vec2 target) {
    double aimX;
    double aimY;

    switch (this->aimOffsetX) {
        case eAnchorX::left:
            aimX = this->bounds.center.x - this->bounds.halfWidth;
            break;
        case eAnchorX::middle:
            aimX = this->bounds.center.x;
            break;
        case eAnchorX::right:
            aimX = this->bounds.center.x + this->bounds.halfWidth;
            break;
        default:
            throw std::logic_error("An object's aimOffsetX is invalid (somehow).");
    }
    switch (this->aimOffsetY) {
        case eAnchorY::top:
            aimY = this->bounds.center.y - this->bounds.halfHeight;
            break;
        case eAnchorY::middle:
            aimY = this->bounds.center.y;
            break;
        case eAnchorY::bottom:
            aimY = this->bounds.center.y + this->bounds.halfHeight;
            break;
        default:
            throw std::logic_error("An object's aimOffsetY is invalid (somehow).");
    }

    this->aimDirection = {target.x - aimX, target.y - aimY};
    this->aimDirection = this->aimDirection.normalized();
}

GameObject::~GameObject() {};

/* -- Player -- */

// Constructors
Player::Player() {
    this->bounds = AABB(this, {0, 0}, PLR_WIDTH/2, PLR_HEIGHT/2);
    this->anchorOffsetX = eAnchorX::middle;
    this->anchorOffsetY = eAnchorY::bottom;
    this->moveSpeed = PLR_MOVESPEED;
    this->state = "stand";
    this->direction = DIR_RIGHT;
    this->directionType = eDirTypes::horizontal;
    this->aimOffsetX = eAnchorX::middle;
    this->aimOffsetY = eAnchorY::middle;
}
Player::Player(double x, double y)
    : Player() {
    this->teleport(x, y);
}

/* -- Projectile -- */

// Constructors
Projectile::Projectile() {
    this->directionType = eDirTypes::omni;
    this->weight = 0;
}
Projectile::Projectile(GameObject* owner, double width, double height)
    : Projectile() {
    this->owner = owner;
    this->bounds.halfWidth = width/2;
    this->bounds.halfHeight = height/2;
    
    if (owner != nullptr) {
        this->teleport(owner->getX(), owner->getY());
    }
}