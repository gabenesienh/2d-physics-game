// Definitions for classes related to level tiles, and the available tile types

#ifndef TILES_HPP
#define TILES_HPP

#include <string>
#include <unordered_map>

#include "util.hpp"

using std::string;
using std::unordered_map;

const int TILEGRID_CELL_SIZE = 32;

class Tile;

// Tile bounding box
// "grid" attributes are measured in grid cells
struct TileBB : public BoundingBox {
    const Tile* parent = nullptr;
    int         gridX;
    int         gridY;

    TileBB(Tile* parent, int gridX, int gridY);

    // Gets the X or Y coordinate at the very edge of the specified side
    double getTopY() const;
    double getBottomY() const;
    double getLeftX() const;
    double getRightX() const;
};

// For use with Tile class
// Defines common properties for all instances of Tile with the same TileType
// "grid" attributes are measured in grid cells
struct TileType {
    const int gridWidth;
    const int gridHeight;

    TileType(int gridWidth, int gridHeight);
};

// Represents a single tile in a level
class Tile {
    private:
        int    typeId; // References tileTypesTable
        TileBB bounds; // Generated when constructing, based on typeId
    public:
        Tile(int typeId, int gridX, int gridY);
        
        int     getTypeId() const;
        TileBB& getBounds();

        // The tile's bounds' X/Y, multiplied by the grid's cell size
        int getX() const;
        int getY() const;

        // The tile's type's width/height, multiplied by the grid's cell size
        int getWidth() const;
        int getHeight() const;
};

// All TileType definitions go here
// A TileType's key in this table will be its ID
extern const unordered_map<int, TileType> tileTypesTable;

#endif