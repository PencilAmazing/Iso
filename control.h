#pragma once

#include "tileset.h"
#include <vector>

struct {
    bool MountainTool = true; // Should smooth neighbors
    int Size = 0; // 0 means tile/corner level selection
} ControlSettings;

// 0 == floor
// 1 == slope
// 2 == raised
// Give center of tile, raises neighbors accordingly

void RaiseMountain(int i, int j, TileMap& heightmap)
{
    MapTile center = heightmap[i][j];

    // North tile
    //if (heightmap[i][j - 1].height == center.height) {
    heightmap[i][j - 1].corners = BOTTOM_RIGHT_CORNER | BOTTOM_LEFT_CORNER;
    //}
    // South tile
    //if (heightmap[i][j + 1].height == center.height) {
    heightmap[i][j + 1].corners = TOP_RIGHT_CORNER | TOP_LEFT_CORNER;
    //}
    heightmap[i - 1][j].corners = TOP_LEFT_CORNER | BOTTOM_LEFT_CORNER;
    heightmap[i + 1][j].corners = TOP_RIGHT_CORNER| BOTTOM_RIGHT_CORNER;
}

void RaiseTerrain(int x, int y, TileMap& heightmap)
{
    // Bounds check
    if (x < 0 || x >= mapwidth || y < 0 || y >= mapheight) return;

    int selectedHeight = heightmap[x][y].height;
    // Clamp height (for now)
    if (selectedHeight >= 2) {
        heightmap[x][y].height = 2;
        return;
    }

    heightmap[x][y].height += 1;

    // If terrain is flat, make neighbors slopes
    // Works on flat terrain for now
    if (ControlSettings.MountainTool && ReadTile(heightmap[x][y]).elevation == TileElevation::Flat) {
        RaiseMountain(x, y, heightmap);
    }
}
