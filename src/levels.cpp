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
Level::Level(string displayName, vector<Tile> tiles)
    : displayName(displayName),
      tiles(tiles) {
}

// Getters
string        Level::getDisplayName() const { return this->displayName; }
vector<Tile>& Level::getTiles()             { return this->tiles; }

const unordered_map<string, Level> levelsTable = {
    {"test", Level(
        "Test",
        {
            Tile(
                1,
                6,
                13
            ),
            Tile(
                1,
                7,
                13
            ),
            Tile(
                1,
                8,
                13
            ),
            Tile(
                1,
                9,
                13
            ),
            Tile(
                1,
                10,
                13
            ),
            Tile(
                1,
                10,
                12
            ),
            Tile(
                1,
                10,
                11
            ),
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
            ),
            Tile(
                1,
                20,
                11
            ),
            Tile(
                1,
                20,
                12
            ),
            Tile(
                1,
                20,
                13
            ),
            Tile(
                1,
                20,
                14
            ),
            Tile(
                1,
                21,
                14
            ),
            Tile(
                1,
                22,
                14
            ),
            Tile(
                1,
                23,
                14
            ),
            Tile(
                1,
                24,
                14
            )
        }
    )}
};

