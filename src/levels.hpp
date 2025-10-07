// The Level class and related structures

#ifndef LEVELS_HPP
#define LEVELS_HPP

#include <string>
#include <unordered_map>
#include <vector>

#include "tiles.hpp"

using std::string;
using std::unordered_map;
using std::vector;

// A group of tiles which represents a playable level
class Level {
    private:
        string       displayName;
        vector<Tile> tiles;
    public:
        Level(string displayName, vector<Tile> tiles);

        string       getDisplayName() const;
        vector<Tile> getTiles() const;
};

// All Level definitions go here
extern const unordered_map<string, Level> levelsTable;

#endif