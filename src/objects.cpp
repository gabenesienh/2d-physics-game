//TODO: validate position on GameObject::tryMove
//TODO: validate screen position getters for GameObject
//TODO: validate GameObject::isVisible

#include "objects.hpp"

#include <cmath>
#include <string>
#include <stdexcept>

#include "tiles.hpp"
#include "util.hpp"

using std::abs, std::min;
using std::string;

/* -- AABB -- */

// Constructors
AABB::AABB(vec2<double> center, double halfWidth, double halfHeight)
    : center(center),
      halfWidth(halfWidth),
      halfHeight(halfHeight) {}

// Getters
double AABB::getTopY() const    { return this->center.y - this->halfHeight; }
double AABB::getBottomY() const { return this->center.y + this->halfHeight; }
double AABB::getLeftX() const   { return this->center.x - this->halfWidth; }
double AABB::getRightX() const  { return this->center.x + this->halfWidth; }

/* -- GameObject -- */

/* 
 * IMPORTANT: For most cases, even internally, you do not want to manipulate
 * this->bounds.center, since it refers to the bounding box's center,
 * without accounting for the object's pivot! Instead, use the proper getters
 * and setters
 */

// Getters
AABB&        GameObject::getBounds()              { return this->bounds; }
double       GameObject::getPivotX() const        { return this->pivotX; }
double       GameObject::getPivotY() const        { return this->pivotY; }
double       GameObject::getSpeedX() const        { return this->speedX; }
double       GameObject::getSpeedY() const        { return this->speedY; }
double       GameObject::getMoveSpeed() const     { return this->moveSpeed; }
string       GameObject::getState() const         { return this->state; }
vec2<double> GameObject::getDirection() const     { return this->direction; }
eDirTypes    GameObject::getDirectionType() const { return this->directionType; }
double       GameObject::getWeight() const        { return this->weight; }
vec2<double> GameObject::getAimDirection() const  { return this->aimDirection; }
double       GameObject::getAimOriginX() const    { return this->aimOriginX; }
double       GameObject::getAimOriginY() const    { return this->aimOriginY; }
int          GameObject::getHealth() const        { return this->health; }

double GameObject::getX() const {
    return this->bounds.center.x + this->bounds.halfWidth*this->pivotX;
}
double GameObject::getY() const {
    return this->bounds.center.y + this->bounds.halfHeight*this->pivotY;
}
double GameObject::getWidth() const {
    return this->bounds.halfWidth*2;
}
double GameObject::getHeight() const {
    return this->bounds.halfHeight*2;
}
double GameObject::getAimX() const {
    return this->bounds.center.x + this->bounds.halfWidth*this->aimOriginX;
}
double GameObject::getAimY() const {
    return this->bounds.center.y + this->bounds.halfHeight*this->aimOriginY;
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
bool GameObject::setDirection(vec2<double> direction) {
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
    double destX = x - this->bounds.halfWidth*this->pivotX;
    double destY = y - this->bounds.halfHeight*this->pivotY;

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
    if (this->walkType == eWalkTypes::aerial) {
        this->speedY = this->direction.y * this->moveSpeed;
    }
}
void GameObject::walk(vec2<double> direction) {
    direction = direction.normalized();

    this->speedX = direction.x * this->moveSpeed;
    if (this->walkType == eWalkTypes::aerial) {
        this->speedY = direction.y * this->moveSpeed;
    }
}

void GameObject::aimAt(vec2<double> target) {
    double aimX = this->bounds.center.x + this->bounds.halfWidth*this->aimOriginX;
    double aimY = this->bounds.center.y + this->bounds.halfHeight*this->aimOriginY;

    this->aimDirection = {target.x - aimX, target.y - aimY};
    this->aimDirection = this->aimDirection.normalized();
}

void GameObject::onCollideTile(Tile* tile, vec2<int> intersection) {
    //TEMP: teleport the object to the tile's top, regardless of collision angle
    double destY = tile->getBounds().getTopY();

    // Adjust for the object's Y pivot
    destY += this->bounds.halfHeight*(this->pivotY - 1);

    this->teleport(this->getX(), destY);
    this->speedY = 0;

    //TEMP: set grounded to true regardless of collision angle
    this->grounded = true;
}

GameObject::~GameObject() {};

/* -- Player -- */

// Constructors
Player::Player(double x, double y) {
    this->bounds = AABB({0, 0}, PLR_WIDTH/2, PLR_HEIGHT/2);
    this->moveSpeed = PLR_MOVESPEED;
    this->state = "stand";
    this->direction = DIR_RIGHT;
    this->directionType = eDirTypes::horizontal;
    this->teleport(x, y);
}

// Getters
eObjTypes Player::getObjectType() {
    return eObjTypes::player;
}

/* -- Projectile -- */

// Constructors
Projectile::Projectile() {
    this->directionType = eDirTypes::omni;
    this->weight = 0;
}
Projectile::Projectile(GameObject* owner, int lifespan, double width, double height)
    : Projectile() {
    this->owner = owner;
    this->lifespan = lifespan;
    this->bounds.halfWidth = width/2;
    this->bounds.halfHeight = height/2;
    
    if (owner != nullptr) {
        this->teleport(owner->getX(), owner->getY());
    }
}

// Getters
eObjTypes Projectile::getObjectType() {
    return eObjTypes::projectile;
}

// Other methods
void Projectile::tick() {
    // Tick down the projectile's lifespan, if it's not negative
    if (this->lifespan == 0) {
        this->health = 0;
    } else if (this->lifespan > 0) {
        this->lifespan--;
    }

    if (this->grounded) {
        if (this->speedX != 0) {
            /* -- Friction -- */

            // How much speed should be lost due to friction
            double spdReduction = abs(this->speedX)*this->frictionMult + this->frictionAdd;
    
            // Directional factor to apply the speed loss to
            int dir = (this->speedX > 0) ? 1 : -1;
    
            // Apply and cap the speed loss so the object isn't thrust backwards
            this->speedX -= min(spdReduction, abs(this->speedX))*dir;
        }

        this->grounded = false;
    }
}