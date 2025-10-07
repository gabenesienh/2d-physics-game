#include "quadtree.hpp"

#include <array>
#include <vector>

#include "objects.hpp"

using std::array;
using std::vector;

/* -- QuadTree -- */

// Constructors
QuadTree::QuadTree(AABB bounds)
    : bounds(bounds) {}

// Getters
AABB&               QuadTree::getBounds()          { return this->bounds; }
vector<AABB*>       QuadTree::getItems() const     { return this->items; }
array<QuadTree*, 4> QuadTree::getQuadrants() const { return this->quads; }

// Other methods
void QuadTree::clear() {
    // Delete the item pointers in this node
    // Will NOT free the items that the pointers point to
    this->items.clear();

    // Recursively delete all items contained in this node's quadrants
    // Also delete the quadrants themselves
    if (this->quads[0] != nullptr) {
        for (int i = 0; i < this->quads.size(); i++) {
            this->quads[i]->clear();
            delete(this->quads[i]);
            this->quads[i] = nullptr;
        }
    }
}

void QuadTree::subdivide() {
    vec2 nwCenter = {
        this->bounds.center.x - this->bounds.halfWidth/2,
        this->bounds.center.y - this->bounds.halfHeight/2
    };
    vec2 neCenter = {
        this->bounds.center.x + this->bounds.halfWidth/2,
        this->bounds.center.y - this->bounds.halfHeight/2
    };
    vec2 swCenter = {
        this->bounds.center.x - this->bounds.halfWidth/2,
        this->bounds.center.y + this->bounds.halfHeight/2
    };
    vec2 seCenter = {
        this->bounds.center.x + this->bounds.halfWidth/2,
        this->bounds.center.y + this->bounds.halfHeight/2
    };

    this->quads[0] = new QuadTree(
        AABB(nullptr, nwCenter, this->bounds.halfWidth/2, this->bounds.halfHeight/2)
    );
    this->quads[1] = new QuadTree(
        AABB(nullptr, neCenter, this->bounds.halfWidth/2, this->bounds.halfHeight/2)
    );
    this->quads[2] = new QuadTree(
        AABB(nullptr, swCenter, this->bounds.halfWidth/2, this->bounds.halfHeight/2)
    );
    this->quads[3] = new QuadTree(
        AABB(nullptr, seCenter, this->bounds.halfWidth/2, this->bounds.halfHeight/2)
    );
}

int QuadTree::findFittingQuadrant(AABB& box) const {
    bool fitsNorth = false;
    bool fitsSouth = false;
    bool fitsWest = false;
    bool fitsEast = false;

    // Check if the given box fits any possible halves of this node's bounds
    // In theory, no more than 2 of these should be true at the same time
    if (box.getTopY() >= this->bounds.getTopY()
    &&  box.getBottomY() <= this->bounds.center.y) {
        fitsNorth = true;
    }
    if (box.getTopY() >= this->bounds.center.y
    &&  box.getBottomY() <= this->bounds.getBottomY()) {
        fitsSouth = true;
    }
    if (box.getLeftX() >= this->bounds.getLeftX()
    &&  box.getRightX() <= this->bounds.center.x) {
        fitsWest = true;
    }
    if (box.getLeftX() >= this->bounds.center.x
    &&  box.getRightX() <= this->bounds.getRightX()) {
        fitsEast = true;
    }

    // If the item fits a vertical and a horizontal half, that's a quadrant
    // Return the this->quads index of the quadrant in question
    if (fitsNorth) {
        if (fitsWest) {
            return QUAD_NW;
        }
        if (fitsEast) {
            return QUAD_NE;
        }
    }
    if (fitsSouth) {
        if (fitsWest) {
            return QUAD_SW;
        }
        if (fitsEast) {
            return QUAD_SE;
        }
    }

    return -1;
}

void QuadTree::insert(AABB& box) {
    // Does this node already have quadrants generated?
    if (this->quads[0] != nullptr) {
        int fitsIndex = this->findFittingQuadrant(box);

        // Insert this box into a quadrant instead, if there's one that fits it
        if (fitsIndex != -1) {
            this->quads[fitsIndex]->insert(box);
            return;
        }
    }

    // No quads generated or it doesn't fit into any of them, so insert it into
    // this node
    this->items.push_back(&box);

    // Split this node if it's now above capacity
    if (this->items.size() > QuadTree::BUCKET_CAPACITY) {
        if (this->quads[0] == nullptr) {
            this->subdivide();
        }

        /*
         * Move this node's items into its newly-generated quadrants, if
         * they fit
         *
         * This looks like it should be a for loop. However, once an element is
         * erased from this->items, the elements behind it get shifted forward
         * by one index, so the index should not increment in such a scenario
         */
        int i = 0;

        while (i < this->items.size()) {
            int fitsIndex = this->findFittingQuadrant(*(this->items[i]));

            if (fitsIndex != -1) {
                this->quads[fitsIndex]->insert(*(this->items[i]));
                this->items.erase(this->items.begin() + i);
                continue;
            }

            i++;
        }
    }
}
vector<AABB*> QuadTree::findPossibleCollisions(
    AABB& box,
    vector<AABB*> acc
) const {
    // acc is an accumulator with all items that could collide with this AABB

    // Recursively add items from other quadrants this AABB fits into
    if (this->quads[0] != nullptr) {
        int index = this->findFittingQuadrant(box);

        if (index != -1) {
            acc = this->quads[index]->findPossibleCollisions(box, acc);
        }
    }

    // Add all items from this current node
    for (AABB* box : this->items) {
        acc.push_back(box);
    }

    return acc;
}