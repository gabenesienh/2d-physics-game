//TODO: scale trees' bounds with currently loaded level
//TODO: proper ground collision

#include "game.hpp"

#include <SDL2/SDL.h>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

#include "events.hpp"
#include "levels.hpp"
#include "objects.hpp"
#include "preferences.hpp"
#include "quadtree.hpp"
#include "tiles.hpp"
#include "util.hpp"

using std::min;
using std::cin, std::cout, std::endl;
using std::setw;
using std::string;
using std::vector;

// Usage: speedY += speedY*GRAV_MULT + GRAV_ADD
const double GRAV_ADD = 0.15;
const double GRAV_MULT = 0.05;
const double GRAV_CAP = 10;

// Loads the specified level from levelsTable
Level* loadLevel(string levelName);

// Kills the object of the specified index, removing it from gameObjects
void killGameObject(int index);

// Clears and repopulates gameObjectsTree
void rebuildGameObjectsTree();

// Sends debug info to standard output, based on the value of debugMode
void printDebugInfo();

int gameState = GS_LAUNCHED;

int    debugMode = 0;
double debugOutputTimer = 0; // Used for delaying std::cout

Player* player;
Level*  loadedLevel; // Receives a value upon calling loadLevel

vector<GameObject*> gameObjects = {};

QuadTree* gameObjectsTree = new QuadTree(
    0,
    AABB(
        nullptr,
        {WINDOW_WIDTH/2, WINDOW_HEIGHT/2},
        (WINDOW_WIDTH/2) - 2,
        (WINDOW_HEIGHT/2) - 2
    )
);

QuadTree* tilesTree = new QuadTree(
    0,
    AABB(
        nullptr,
        {WINDOW_WIDTH/2, WINDOW_HEIGHT/2},
        (WINDOW_WIDTH/2) - 2,
        (WINDOW_HEIGHT/2) - 2
    )
);

array<bool, 5> mouseStatesTap = {false}; // Stores previous frame's mouseStates

void doGame() {
switch (gameState) {
case GS_LAUNCHED:
    // Load level
    loadedLevel = loadLevel("test");

    for (Tile& tile : loadedLevel->getTiles()) {
        tilesTree->insert(tile.getBounds());
    }

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

    //TEMP: Fire projectiles with M1
    if (mouseStates[SDL_BUTTON_LEFT]) {
        Projectile* proj = new Projectile(player, 40);
        proj->setWidth(15);
        proj->setHeight(15);
        proj->teleport(
            player->getX(),
            player->getBounds().center.y
        );
        proj->setWeight(0);
        proj->thrust(
            8 * player->getAimDirection().x,
            8 * player->getAimDirection().y
        );

        gameObjects.push_back(proj);
    }

    /* -- Physics -- */

    rebuildGameObjectsTree();

    // The loop below sometimes requires the GameObject's index in gameObjects,
    // therefore a forEach can't be used
    // There's also a chance the object will be killed and removed from
    // gameObjects, in which case the iterator should not increment
    int i = 0;

    while (i < gameObjects.size()) {
        GameObject* gobj = gameObjects[i]; // For convenience

        // Apply gravity to objects
        gobj->thrust(
            0,
            (gobj->getSpeedY()*GRAV_MULT + GRAV_ADD)*gobj->getWeight()
        );

        // Cap falling speed
        gobj->setSpeedY(min(gobj->getSpeedY(), GRAV_CAP));

        // Displace game objects based on their speed
        double targetX = gobj->getX() + gobj->getSpeedX();
        double targetY = gobj->getY() + gobj->getSpeedY();

        gobj->tryMove(targetX, targetY);

        // Run the object's specific logic for this tick
        gobj->tick();

        // Kill object if it's out of health
        if (gobj->getHealth() <= 0) {
            killGameObject(i);
            continue;
        }

        i++;
    }

    /* -- Collision -- */

    rebuildGameObjectsTree();

    for (GameObject* gobj : gameObjects) {
        // Find all tiles that could possibly be colliding with this object
        vector<BoundingBox*> possibleCols;
        possibleCols = tilesTree->findPossibleCollisions(gobj->getBounds());

        for (BoundingBox* possibleCol : possibleCols) {
            if (gobj->getBounds().intersects(*possibleCol)) {
                gobj->teleport(
                    gobj->getX(),
                    320
                );
                gobj->setSpeedY(0);
            }
        }
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

void killGameObject(int index) {
    delete(gameObjects[index]);
    gameObjects.erase(gameObjects.begin() + index);
}

void rebuildGameObjectsTree() {
    gameObjectsTree->clear();

    for (GameObject* gobj : gameObjects) {
        gameObjectsTree->insert(gobj->getBounds());
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
