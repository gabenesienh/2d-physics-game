// Main game logic

#ifndef GAME_HPP
#define GAME_HPP

#include <vector>

#include "levels.hpp"
#include "objects.hpp"
#include "quadtree.hpp"
#include "tiles.hpp"

using std::vector;

// Possible game states
const int GS_LAUNCHED = 0;
const int GS_STARTED = 1;

// Flags for use with debugMode
const int DEBUG_CONFIGS          = 0b0000001;
const int DEBUG_PERFORMANCE_INFO = 0b0000010;
const int DEBUG_LEVEL_INFO       = 0b0000100;
const int DEBUG_PLAYER_INFO      = 0b0001000;
const int DEBUG_SHOW_HITBOXES    = 0b0010000;
const int DEBUG_SHOW_QUADS       = 0b0100000;
const int DEBUG_SUBTICK_RENDERS  = 0b1000000;

// Enables debug features
// To be used with DEBUG_* flags
extern int debugMode;

// Delta time, measured in 60ths of a second
// Should be capped at 1
extern double dt;

// The current game state, uses GS_* constants
extern int gameState;

// Points to currently loaded level
extern Level* loadedLevel;

// The objects currently present in the game
extern vector<GameObject*> gameObjects;

// Tree structure containing pointers to the bounding boxes of all objects
// currently in-game
extern QuadTree<GameObject>* gameObjectsTree;

// Tree structure containing pointers to the bounding boxes of all level tiles
// currently loaded
extern QuadTree<Tile>* tilesTree;

// The player object in gameObjects
extern Player* player;

// Processes game logic for a frame
extern void doGame();

#endif
