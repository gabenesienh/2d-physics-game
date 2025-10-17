#include "tiles.hpp"

#include "util.hpp"

/* -- TileAABB -- */

// Constructors
TileAABB::TileAABB(int typeId, int gridX, int gridY)
    : typeId(typeId),
      gridX(gridX),
      gridY(gridY) {}

// Getters
double TileAABB::getTopY() const {
    return this->gridY*TILEGRID_CELL_SIZE;
}
double TileAABB::getBottomY() const {
    int gridHeight = tileTypesTable.at(this->typeId).gridHeight;
    return (this->gridY + gridHeight)*TILEGRID_CELL_SIZE;
}
double TileAABB::getLeftX() const {
    return this->gridX*TILEGRID_CELL_SIZE;
}
double TileAABB::getRightX() const {
    int gridWidth = tileTypesTable.at(this->typeId).gridWidth;
    return (this->gridX + gridWidth)*TILEGRID_CELL_SIZE;
}

/* -- TileType -- */

// Constructors
TileType::TileType(int gridWidth, int gridHeight)
    : gridWidth(gridWidth),
      gridHeight(gridHeight) {}

/* -- Tile -- */

// Constructors
Tile::Tile(int typeId, int gridX, int gridY)
    : bounds(TileAABB(typeId, gridX, gridY)) {}

// Getters
TileAABB& Tile::getBounds()       { return this->bounds; }

// Other methods
int Tile::getX() const {
    return this->bounds.gridX*TILEGRID_CELL_SIZE;
}
int Tile::getY() const {
    return this->bounds.gridY*TILEGRID_CELL_SIZE;
}
double Tile::getWidth() const {
    return tileTypesTable.at(this->bounds.typeId).gridWidth*TILEGRID_CELL_SIZE;
}
double Tile::getHeight() const {
    return tileTypesTable.at(this->bounds.typeId).gridHeight*TILEGRID_CELL_SIZE;
}

// View the header file for info on constructor structures

const unordered_map<int, TileType> tileTypesTable = {
    {1, TileType(
        1,
        1
    )}
};
