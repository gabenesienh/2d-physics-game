//TODO: stop rendering objects out of view
//TODO: refactor color setting code

#include "graphics.hpp"

#include <SDL2/SDL.h>
#include <cmath>

#include "events.hpp"
#include "game.hpp"
#include "tiles.hpp"
#include "quadtree.hpp"
#include "util.hpp"

using std::abs, std::max;

// Set colors if they're not already set
void setColors();

// Recursively draw a QuadTree
template <typename T>
void drawTree(QuadTree<T>* tree, Uint32 color);

// Used for rendering game objects as solid rectangles in debug mode
// X and Y refer to screen position rather than game position
// Width and height are adjusted per object while rendering
SDL_Rect rendererRect;

// Whether or not the necessary colors have been mapped yet
// setColors should not be called if this is true
bool isColorsSet = false;

Uint32 blackColor;

Uint32 debugTileColor;
Uint32 debugHitboxColor;
Uint32 debugAnchorColor;
Uint32 debugDirectionColor;
Uint32 debugPlayerAimColor;
Uint32 debugObjTreeColor;
Uint32 debugTileTreeColor;

void doRender() {
    if (!isColorsSet) setColors();

    // Clear screen before drawing
    SDL_FillRect(gameSurface, NULL, blackColor);

    for (Tile& tile : loadedLevel->getTiles()) {
        if (debugMode & DEBUG_SHOW_HITBOXES) {
            /* -- Draw tile collision boxes -- */

            rendererRect.w = tile.getWidth();
            rendererRect.h = tile.getHeight();
            rendererRect.x = tile.getX();
            rendererRect.y = tile.getY();

            SDL_FillRect(gameSurface, &rendererRect, debugTileColor);
        }
    }

    if (debugMode & DEBUG_SHOW_QUADS) {
        // Show boundaries of the collision trees
        drawTree(tilesTree, debugTileTreeColor);
        drawTree(gameObjectsTree, debugObjTreeColor);
    }

    for (GameObject* gobj : gameObjects) {
        if (!gobj->isVisible()) return;

        if (debugMode & DEBUG_SHOW_HITBOXES) {
            /* -- Render object hitbox -- */
    
            rendererRect.w = gobj->getWidth();
            rendererRect.h = gobj->getHeight();

            switch (gobj->getAnchorOffsetX()) {
                case eAnchorX::left:
                    rendererRect.x = gobj->getScreenX();
                    break;
                case eAnchorX::middle:
                    rendererRect.x = gobj->getScreenX() - rendererRect.w/2;
                    break;
                case eAnchorX::right:
                    rendererRect.x = gobj->getScreenX() - rendererRect.w;
                    break;
            }
            switch (gobj->getAnchorOffsetY()) {
                case eAnchorY::top:
                    rendererRect.y = gobj->getScreenY();
                    break;
                case eAnchorY::middle:
                    rendererRect.y = gobj->getScreenY() - rendererRect.h/2;
                    break;
                case eAnchorY::bottom:
                    rendererRect.y = gobj->getScreenY() - rendererRect.h;
                    break;
            }

            SDL_FillRect(gameSurface, &rendererRect, debugHitboxColor);
    
            /* -- Draw line from object's center to their direction -- */
    
            rendererRect.w = 1;
            rendererRect.h = 1;
    
            // The starting point of the line (object's AABB's center)
            int centerX = gobj->getBounds().center.x - rendererRect.w/2;
            int centerY = gobj->getBounds().center.y - rendererRect.h/2;
    
            // The ending point of the line
            int targetX = centerX + (gobj->getDirection().x * 25);
            int targetY = centerY + (gobj->getDirection().y * 25);
    
            drawLine(
                gameSurface, rendererRect, debugDirectionColor,
                centerX, centerY,
                targetX, targetY
            );
    
            /* -- Draw line from player object(s) to cursor -- */

            if (gobj == player) {
                drawLine(
                    gameSurface, rendererRect, debugPlayerAimColor,
                    centerX, centerY,
                    mouseScreenPos.x, mouseScreenPos.y
                );
            }
    
            /* -- Draw object's anchor point -- */
    
            rendererRect.w = 4;
            rendererRect.h = 4;
            rendererRect.x = gobj->getScreenX() - rendererRect.w/2;
            rendererRect.y = gobj->getScreenY() - rendererRect.h/2;
            SDL_FillRect(gameSurface, &rendererRect, debugAnchorColor);
        }
    }

    SDL_BlitSurface(gameSurface, NULL, winSurface, NULL);
    SDL_UpdateWindowSurface(window);
}

void drawLine(SDL_Surface* surface, SDL_Rect& rendererRect, Uint32 color, int x0, int y0, int x1, int y1) {
    // Deltas
    int dx = x1 - x0;
    int dy = y1 - y0;

    // The magnitude of the delta whose magnitude is the largest
    // This will help determine which octant the line is in
    double step = max(abs(dx), abs(dy));

    // How much X and Y should change with each "step" from (x0,y0) to (x1,y1)
    // Depending on the octant, one of these will always be 1 or -1
    double stepX = dx / step;
    double stepY = dy / step;

    for (int i = 0; i <= step; i++) {
        if (step == 0) break; // Avoid division by zero error

        rendererRect.x = x0 + stepX*i;
        rendererRect.y = y0 + stepY*i;

        SDL_FillRect(surface, &rendererRect, color);
    }
}

void drawRectangle(SDL_Surface* surface, SDL_Rect& rendererRect, Uint32 color, int x0, int y0, int x1, int y1) {
    drawLine(surface, rendererRect, color, x0, y0, x1, y0);
    drawLine(surface, rendererRect, color, x1, y0, x1, y1);
    drawLine(surface, rendererRect, color, x1, y1, x0, y1);
    drawLine(surface, rendererRect, color, x0, y1, x0, y0);
}

template <typename T>
void drawTree(QuadTree<T>* tree, Uint32 color) {
    rendererRect.w = 1;
    rendererRect.h = 1;

    AABB& bounds = tree->getBounds();

    drawRectangle(
        gameSurface, rendererRect, color,
        bounds.center.x - bounds.halfWidth,
        bounds.center.y - bounds.halfHeight,
        bounds.center.x + bounds.halfWidth,
        bounds.center.y + bounds.halfHeight
    );

    if (tree->getQuadrants()[0] != nullptr) {
        for (QuadTree<T>* quad : tree->getQuadrants()) {
            drawTree(quad, color);
        }
    }
}

// Necessary due to surface formats being unknown before util.cpp is run
void setColors() {
    blackColor = SDL_MapRGB(
        gameSurface->format,
        0,
        0,
        0
    );

    debugTileColor = SDL_MapRGB(
        gameSurface->format,
        255,
        255,
        255
    );

    debugHitboxColor = SDL_MapRGB(
        gameSurface->format,
        255,
        31,
        31
    );

    debugAnchorColor = SDL_MapRGB(
        gameSurface->format,
        31,
        255,
        255
    );

    debugDirectionColor = SDL_MapRGB(
        gameSurface->format,
        255,
        127,
        31
    );

    debugPlayerAimColor = SDL_MapRGB(
        gameSurface->format,
        31,
        255,
        31
    );

    debugObjTreeColor = SDL_MapRGB(
        gameSurface->format,
        255,
        127,
        63
    );

    debugTileTreeColor = SDL_MapRGB(
        gameSurface->format,
        63,
        127,
        0
    );

    isColorsSet = true;
}
