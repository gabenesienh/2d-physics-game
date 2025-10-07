#include "levels.hpp"

#include <string>
#include <unordered_map>
#include <vector>

#include "tiles.hpp"

using std::string;
using std::unordered_map;
using std::vector;

/* -- Level -- */

// Constructors
Level::Level(string displayName, vector<Tile> tiles) {
    this->displayName = displayName;
    this->tiles = tiles;
}

// Getters
string       Level::getDisplayName() const { return this->displayName; }
vector<Tile> Level::getTiles() const       { return this->tiles; }

const unordered_map<string, Level> levelsTable = {
    {"test", Level(
        "Test",
        {
            Tile(
                1,
                10,
                10
            ),
            Tile(
                1,
                11,
                10
            ),
            Tile(
                1,
                12,
                10
            ),
            Tile(
                1,
                13,
                10
            ),
            Tile(
                1,
                14,
                10
            ),
            Tile(
                1,
                15,
                10
            ),
            Tile(
                1,
                16,
                10
            ),
            Tile(
                1,
                17,
                10
            ),
            Tile(
                1,
                18,
                10
            ),
            Tile(
                1,
                19,
                10
            ),
            Tile(
                1,
                20,
                10
            )
        }
    )}
};