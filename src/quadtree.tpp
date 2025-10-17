#include "quadtree.hpp"

#include <array>
#include <vector>

#include "objects.hpp"

using std::array;
using std::vector;

/* -- QuadTree -- */


// Constructors
template<typename T>
QuadTree<T>::QuadTree(int level, AABB bounds)
    : level(level),
      bounds(bounds) {}

// Getters
template<typename T>
AABB&                  QuadTree<T>::getBounds()          { return this->bounds; }
template<typename T>
vector<T*>             QuadTree<T>::getItems() const     { return this->items; }
template<typename T>
array<QuadTree<T>*, 4> QuadTree<T>::getQuadrants() const { return this->quads; }

// Other methods
template<typename T>
void QuadTree<T>::clear() {
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

template<typename T>
void QuadTree<T>::subdivide() {
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
        this->level + 1,
        AABB(nwCenter, this->bounds.halfWidth/2, this->bounds.halfHeight/2)
    );
    this->quads[1] = new QuadTree(
        this->level + 1,
        AABB(neCenter, this->bounds.halfWidth/2, this->bounds.halfHeight/2)
    );
    this->quads[2] = new QuadTree(
        this->level + 1,
        AABB(swCenter, this->bounds.halfWidth/2, this->bounds.halfHeight/2)
    );
    this->quads[3] = new QuadTree(
        this->level + 1,
        AABB(seCenter, this->bounds.halfWidth/2, this->bounds.halfHeight/2)
    );
}

template<typename T>
int QuadTree<T>::findFittingQuadrant(AABBCommon& box) const {
    bool fitsNorth = false;
    bool fitsSouth = false;
    bool fitsWest = false;
    bool fitsEast = false;

    // Check if the given box fits any possible halves of this node's bounds
    // In theory, no more than 2 of these should be true at the same time
    if (box.getBottomY() <= this->bounds.center.y) {
        fitsNorth = true;
    }
    if (box.getTopY() >= this->bounds.center.y) {
        fitsSouth = true;
    }
    if (box.getRightX() <= this->bounds.center.x) {
        fitsWest = true;
    }
    if (box.getLeftX() >= this->bounds.center.x) {
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

template<typename T>
void QuadTree<T>::insert(T* item) {
    // Ignore this item if it's outside the bounds of the root node of the tree
    if (this->level == 0
    &&  item->getBounds().intersects(this->bounds).x == 0) {
        return;
    }

    // Does this node already have quadrants generated?
    if (this->quads[0] != nullptr) {
        int fitsIndex = this->findFittingQuadrant(item->getBounds());

        // Insert this item into a quadrant instead, if there's one that fits it
        if (fitsIndex != -1) {
            this->quads[fitsIndex]->insert(item);
            return;
        }
    }

    // No quads generated or it doesn't fit into any of them, so insert it into
    // this node
    this->items.push_back(item);

    // Split this node if it's now above capacity
    if (this->items.size() > QuadTree::BUCKET_CAPACITY
    &&  this->level < QuadTree::MAX_LEVELS) {
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
            int fitsIndex = this->findFittingQuadrant(this->items[i]->getBounds());

            if (fitsIndex != -1) {
                this->quads[fitsIndex]->insert(this->items[i]);
                this->items.erase(this->items.begin() + i);
                continue;
            }

            i++;
        }
    }
}

template<typename T>
vector<T*> QuadTree<T>::findPossibleCollisions(
    AABBCommon& box,
    vector<T*> acc
) const {
    // acc is an accumulator with all items that could collide with this box

    // Recursively consider items from this node's quadrants, if it has any
    if (this->quads[0] != nullptr) {
        int index = this->findFittingQuadrant(box);

        if (index != -1) {
            // If this box fully fits into a quadrant, consider collisions with
            // only items that are within that quadrant
            acc = this->quads[index]->findPossibleCollisions(box, acc);
        } else {
            // If not, consider collisions with items in all of this node's
            // quadrants
            acc = this->quads[0]->findPossibleCollisions(box, acc);
            acc = this->quads[1]->findPossibleCollisions(box, acc);
            acc = this->quads[2]->findPossibleCollisions(box, acc);
            acc = this->quads[3]->findPossibleCollisions(box, acc);
        }
    }

    // Consider all items from this node
    for (T* item : this->items) {
        acc.push_back(item);
    }

    return acc;
}