//TODO: scale collisionTree bounds with currently loaded level
//TODO: static quadtree for tiles

#include "game.hpp"

#include <SDL2/SDL.h>
#include <cmath>
#include <deque>
#include <iostream>
#include <iomanip>
#include <string>

#include "events.hpp"
#include "levels.hpp"
#include "objects.hpp"
#include "preferences.hpp"
#include "quadtree.hpp"
#include "util.hpp"

using std::min;
using std::deque;
using std::cin, std::cout, std::endl;
using std::setw;
using std::string;

// Usage: speedY += speedY*GRAV_MULT + GRAV_ADD
const double GRAV_ADD = 0.15;
const double GRAV_MULT = 0.05;
const double GRAV_CAP = 10;

// Loads the specified level from levelsTable
Level* loadLevel(string levelName);

// Sends debug info to standard output, based on the value of debugMode
void printDebugInfo();

int debugMode = 0;

// Counts up to 1 and then resets to zero
// Used for delaying debug output
double debugOutputTimer = 0;

int gameState = GS_LAUNCHED;

Player* player;

// Points to a copy of a level from levelsTable
// Receives a value upon calling loadLevel
Level* loadedLevel;

deque<GameObject*> gameObjects = {};

QuadTree* collisionTree = new QuadTree(
    AABB(
        nullptr,
        {WINDOW_WIDTH/2, WINDOW_HEIGHT/2},
        (WINDOW_WIDTH/2) - 2,
        (WINDOW_HEIGHT/2) - 2
    )
);

// Stores last frame's mouseStates
// Used to detect presses and releases
array<bool, 5> mouseStatesTap = {false};

void doGame() {
switch (gameState) {
case GS_LAUNCHED:
    // Load level
    loadedLevel = loadLevel("test");

    // Spawn player
    player = new Player(
        WINDOW_WIDTH/2,
        WINDOW_HEIGHT/2
    );
    gameObjects.push_back(player);

    gameState = GS_STARTED;
    break;
case GS_STARTED:
    /* -- Player input handling -- */

    // Walking
    if (keyStates[BT_LEFT]
    && !keyStates[BT_RIGHT]) {
        player->setState("walk");
        player->walk(DIR_LEFT);
    } else if (keyStates[BT_RIGHT]
           && !keyStates[BT_LEFT]) {
        player->setState("walk");
        player->walk(DIR_RIGHT);
    } else if (player->getState() == "walk") {
        player->setSpeedX(0);
        player->setState("stand");
    }

    // Have the player aim at and face the cursor
    player->aimAt({mouseScreenPos.x, mouseScreenPos.y});

    if (mouseScreenPos.x < player->getScreenX()) {
        player->setDirection(DIR_LEFT);
    } else if (mouseScreenPos.x > player->getScreenX()) {
        player->setDirection(DIR_RIGHT);
    }

    // Fire projectiles with M1
    if (mouseStates[SDL_BUTTON_LEFT]) {
        Projectile* proj = new Projectile(player, 8, 8);
        proj->teleport(
            player->getX(),
            player->getBounds().center.y
        );
        proj->setWeight(0);
        proj->thrust(
            2 * player->getAimDirection().x,
            2 * player->getAimDirection().y
        );

        gameObjects.push_back(proj);
    }

    /* -- Physics -- */

    // Wipe last frame's collision tree
    collisionTree->clear();

    for (GameObject* gobj : gameObjects) {
        // Apply gravity to objects
        gobj->thrust(
            0,
            (gobj->getSpeedY()*GRAV_MULT + GRAV_ADD)*gobj->getWeight()*dt
        );

        // Cap falling speed
        gobj->setSpeedY(min(gobj->getSpeedY(), GRAV_CAP));

        // Displace game objects based on their speed
        double targetX = gobj->getX() + gobj->getSpeedX()*dt;
        double targetY = gobj->getY() + gobj->getSpeedY()*dt;

        gobj->tryMove(targetX, targetY);

        // TEMPORARY INVISIBLE GROUND
        if (gobj->getY() > 320) {
            gobj->teleport(gobj->getX(), 320);
        }

        // Insert object bounding boxes back into the collision tree
        collisionTree->insert(gobj->getBounds());
    }

    /* -- Other -- */

    mouseStatesTap = mouseStates;

    /* -- Debug -- */

    // Show debug info if enabled
    if (debugMode) {
        debugOutputTimer += dt;

        if (debugOutputTimer >= 1) {
            printDebugInfo();
            debugOutputTimer = 0;
        }
    }

    break;
}
}

Level* loadLevel(string levelName) {
    try {
        Level levelToCopy = levelsTable.at(levelName);

        Level* copiedLevel = new Level(
            levelToCopy.getDisplayName(),
            levelToCopy.getTiles()
        );

        return copiedLevel;
    } catch (std::out_of_range e) {
        if (debugMode) {
            cout << "ERROR: Attempted to load level that doesn't exist" << '\n';
        }

        return nullptr;
    }
}

void printDebugInfo() {
    if (debugMode & DEBUG_PERFORMANCE_INFO) {
        cout << setw(10) << "fps="        << setw(16) << static_cast<int>(60/dt) << '\n'
             << '\n';
    }
    if (debugMode & DEBUG_LEVEL_INFO) {
        cout << setw(10) << "lvlname="    << setw(16) << loadedLevel->getDisplayName() << '\n'
             << '\n';
    }
    if (debugMode & DEBUG_PLAYER_INFO) {
        cout << setw(10) << "x="          << setw(16) << player->getX() << '\n'
             << setw(10) << "y="          << setw(16) << player->getY() << '\n'
             << setw(10) << "spdx="       << setw(16) << player->getSpeedX() << '\n'
             << setw(10) << "spdy="       << setw(16) << player->getSpeedY() << '\n'
             << std::showpoint
             << setw(10) << "dirx="       << setw(16) << player->getDirection().x << '\n'
             << setw(10) << "diry="       << setw(16) << player->getDirection().y << '\n'
             << std::showpoint
             << '\n';
    }
}