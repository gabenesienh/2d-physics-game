// Definitions for classes related to level tiles, and the available tile types

#ifndef TILES_HPP
#define TILES_HPP

#include <string>
#include <unordered_map>

using std::string;
using std::unordered_map;

const int TILEGRID_CELL_SIZE = 32;

// For use with Tile class
// Defines common properties for all instances of Tile with the same TileType
// gridWidth and gridHeight are measured in grid cells
class TileType {
    private:
        int gridWidth;
        int gridHeight;
    public:
        TileType(int gridWidth, int gridHeight);

        int getGridWidth() const;
        int getGridHeight() const;

        // Same as getGridW/H but multiplied by the tile grid's cell size
        int getWidth() const;
        int getHeight() const;
};

// Represents a single tile in a level
// gridX and gridY are anchored at the tile's top left corner
// gridX and gridY are measured in grid cells
class Tile {
    private:
        int typeId; //references tileTypesTable
        int gridX;
        int gridY;
    public:
        Tile(int typeId, int gridX, int gridY);
        
        int getTypeId() const;
        int getGridX() const;
        int getGridY() const;

        // Same as getGridX/Y but multiplied by the tile grid's cell size
        int getX() const;
        int getY() const;
};

// All TileType definitions go here
// A TileType's key in this table will be its ID
extern const unordered_map<int, TileType> tileTypesTable;

#endif