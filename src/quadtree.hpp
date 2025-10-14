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

// A quadtree for holding instances of AABBCommon
// Can either be the root node of a quadtree or a quadrant (a nested quadtree)
template <typename T>
class QuadTree {
    private:
        static const int BUCKET_CAPACITY = 4;
        static const int MAX_LEVELS = 10;

        int                 level;
        AABB                bounds;
        vector<T*>          items;
        array<QuadTree*, 4> quads   = {nullptr}; // NW, NE, SW and SE quadrants
    public:
        QuadTree(int level, AABB bounds);

        AABB&               getBounds();
        vector<T*>          getItems() const;
        array<QuadTree*, 4> getQuadrants() const;

        // Clears the items of this node and clear its existing quadrants
        // recursively
        void clear();

        // Generate the NW, NE, SW and SE quadrants, "splitting" this node
        void subdivide();

        // Find the index of whichever quadrant could hold this bounding box
        // Returns -1 on error or if the box can't fully fit into any quadrant
        // PS: will *not* check if the quadrants actually exist! (i.e. if this
        // node has been subdivided)
        int findFittingQuadrant(AABBCommon& box) const;

        // Attempt to insert an item into this node
        // If necessary, this node will be subdivided and all its items will
        // try to fit into a quadrant
        void insert(T* item);

        // Recursively look for items which intersect the given box
        // Returns a list of matched items
        // PS: do *not* specify a value for acc when calling!
        vector<T*> findPossibleCollisions(
            AABBCommon& box,
            vector<T*> acc = {}
        ) const;
};

#include "quadtree.tpp"

#endif
