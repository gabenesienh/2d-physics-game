//TODO: merge TileAABB into Tile...?

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

// For use with Tile class
// Defines common properties for all instances of Tile with the same TileType
// "grid" attributes are measured in grid cells
struct TileType {
    const int gridWidth;
    const int gridHeight;

    TileType(int gridWidth, int gridHeight);
};

// Tile bounding box
// "grid" attributes are measured in grid cells
struct TileAABB : public AABBCommon {
    int typeId; // References tileTypesTable
    int gridX;
    int gridY;

    TileAABB(int typeId, int gridX, int gridY);

    double getTopY() const;
    double getBottomY() const;
    double getLeftX() const;
    double getRightX() const;
};

// Represents a single tile in a level
class Tile {
    private:
        TileAABB bounds; // Generated when constructing, based on typeId given
    public:
        Tile(int typeId, int gridX, int gridY);
        
        TileAABB& getBounds();

        // The tile's bounds' X/Y, multiplied by the grid's cell size
        int getX() const;
        int getY() const;

        // The tile's type's width/height, multiplied by the grid's cell size
        // Typed double for compatibility with other dimension getter methods
        double getWidth() const;
        double getHeight() const;
};

// All TileType definitions go here
// A TileType's key in this table will be its ID
extern const unordered_map<int, TileType> tileTypesTable;

#endif
