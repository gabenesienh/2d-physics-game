#include "tiles.hpp"

/* -- TileType -- */

// Constructors
TileType::TileType(int gridWidth, int gridHeight) {
    this->gridWidth = gridWidth;
    this->gridHeight = gridHeight;
}

// Getters
int TileType::getGridWidth() const  { return this->gridWidth; }
int TileType::getGridHeight() const { return this->gridHeight; }

// Other
int TileType::getWidth() const {
    return this->gridWidth*TILEGRID_CELL_SIZE;
}
int TileType::getHeight() const {
    return this->gridHeight*TILEGRID_CELL_SIZE;
}

/* -- Tile -- */

// Constructors
Tile::Tile(int typeId, int gridX, int gridY) {
    this->typeId = typeId;
    this->gridX = gridX;
    this->gridY = gridY;
}

// Getters
int Tile::getGridX() const      { return this->gridX; }
int Tile::getGridY() const      { return this->gridY; }
int Tile::getTypeId() const { return this->typeId; }

/* -- Other -- */

int Tile::getX() const {
    return this->gridX*TILEGRID_CELL_SIZE;
}
int Tile::getY() const {
    return this->gridY*TILEGRID_CELL_SIZE;
}

// View the header file for info on constructor structures

const unordered_map<int, TileType> tileTypesTable = {
    {1, TileType(
        1,
        1
    )}
};