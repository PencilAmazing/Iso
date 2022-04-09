#pragma once

#include "tileset.h"
#include <vector>


// 0 == floor
// 1 == slope
// 2 == raised

void RaiseTerrain(TileMap& heightmap, int x, int y)
{
    // Bounds check
    if (x < 0 || x >= mapwidth || y < 0 || y >= mapheight) return;

    int selectedHeight = heightmap[x][y].height;
    // Clamp height
    if (selectedHeight >= 2) {
        heightmap[x][y].height = 2;
        return;
    }

    // If terrain is flat, make neighbors slopes
    // If neighbors are already slopes, raise to full height
    // Because saddles are not easy to draw
    heightmap[x][y].height = 2;

    // Make neighbors sloped
    // If neighbor is flat, make sloped
    if (x - 1 >= 0 && heightmap[x - 1][y].height == 0) {
        heightmap[x - 1][y].height = 1;
        heightmap[x - 1][y].direction = TileDirection::West; // Point east
    }
    if (x + 1 < mapwidth && heightmap[x + 1][y].height == 0) {
        heightmap[x + 1][y].height = 1;
        heightmap[x + 1][y].direction = TileDirection::East;
    }
    if (y - 1 >= 0 && heightmap[x][y - 1].height == 0) {
        heightmap[x][y-1].height = 1;
        heightmap[x][y - 1].direction = TileDirection::North;
    }
    if (y + 1 >= 0 && heightmap[x][y + 1].height == 0) {
        heightmap[x][y + 1].height = 1;
        heightmap[x][y + 1].direction = TileDirection::South;
    }
}