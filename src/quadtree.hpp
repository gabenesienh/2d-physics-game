// Definitions for classes that are used for implementing a quadtree structure

#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include <array>
#include <vector>

#include "objects.hpp"
#include "util.hpp"

using std::array;
using std::vector;

const int QUAD_NW = 0;
const int QUAD_NE = 1;
const int QUAD_SW = 2;
const int QUAD_SE = 3;

// A quadtree for holding AABBs
// Can either be the root node of a quadtree or a quadrant (a nested quadtree)
class QuadTree {
    private:
        static const int BUCKET_CAPACITY = 4;

        AABB                bounds;
        vector<AABB*>       items;
        array<QuadTree*, 4> quads   = {nullptr}; // NW, NE, SW and SE quadrants
    public:
        QuadTree(AABB bounds);

        AABB&               getBounds();
        vector<AABB*>       getItems() const;
        array<QuadTree*, 4> getQuadrants() const;

        // Clears the items of this node and clear its existing quadrants
        // recursively
        void clear();

        // Generate the NW, NE, SW and SE quadrants, "splitting" this node
        void subdivide();

        // Find the index of whichever quadrant could hold this bounding box
        // Returns -1 on error or if the box can't fully fit into any quadrant
        // NOTE: will *not* check if the quadrants actually exist! (i.e. if this
        // node has been subdivided)
        int findFittingQuadrant(AABB& box) const;

        // Attempt to insert an item into this node
        // If necessary, this node will be subdivided and all its items will
        // try to fit into a quadrant
        void insert(AABB& box);

        // Recursively look for items (AABBs) which intersect the given box's
        // Returns a list of matched items
        // NOTE: do not specify a value for acc when calling!
        vector<AABB*> findPossibleCollisions(
            AABB& box,
            vector<AABB*> acc = {}
        ) const;
};

#endif