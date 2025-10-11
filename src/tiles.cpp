#include "tiles.hpp"

#include "util.hpp"

/* -- TileBB -- */

// Constructors
TileBB::TileBB(int gridX, int gridY)
    : gridX(gridX),
      gridY(gridY) {}

// Getters
double TileBB::getTopY() const {
    return this->gridY*TILEGRID_CELL_SIZE;
}
double TileBB::getBottomY() const {
    int gridHeight = tileTypesTable.at(this->parent->getTypeId()).gridHeight;
    return (this->gridY + gridHeight)*TILEGRID_CELL_SIZE;
}
double TileBB::getLeftX() const {
    return this->gridX*TILEGRID_CELL_SIZE;
}
double TileBB::getRightX() const {
    int gridWidth = tileTypesTable.at(this->parent->getTypeId()).gridWidth;
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
    : typeId(typeId),
      bounds(TileBB(gridX, gridY)) {}

// Getters
int     Tile::getTypeId() const { return this->typeId; }
TileBB& Tile::getBounds()       { return this->bounds; }

// Other methods
int Tile::getX() const {
    return this->bounds.gridX*TILEGRID_CELL_SIZE;
}
int Tile::getY() const {
    return this->bounds.gridY*TILEGRID_CELL_SIZE;
}
int Tile::getWidth() const {
    return tileTypesTable.at(this->typeId).gridWidth*TILEGRID_CELL_SIZE;
}
int Tile::getHeight() const {
    return tileTypesTable.at(this->typeId).gridHeight*TILEGRID_CELL_SIZE;
}

// View the header file for info on constructor structures

const unordered_map<int, TileType> tileTypesTable = {
    {1, TileType(
        1,
        1
    )}
};
