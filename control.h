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

    heightmap[i][j - 1].corners = tile_element_raise_styles[7][heightmap[i][j - 1].corners];
    heightmap[i][j + 1].corners = tile_element_raise_styles[5][heightmap[i][j + 1].corners];
    heightmap[i - 1][j].corners = tile_element_raise_styles[8][heightmap[i - 1][j].corners];
    heightmap[i + 1][j].corners = tile_element_raise_styles[6][heightmap[i + 1][j].corners];

    //heightmap[i][j - 1].height += 1;
    //heightmap[i][j + 1].height += 1;
    //heightmap[i - 1][j].height += 1;
    //heightmap[i + 1][j].height += 1;

    //heightmap[i - 1][j - 1].corners |= BOTTOM_LEFT_CORNER;
    //heightmap[i - 1][j + 1].corners |= TOP_LEFT_CORNER;
    //heightmap[i + 1][j + 1].corners |= TOP_RIGHT_CORNER;
    //heightmap[i + 1][j - 1].corners |= BOTTOM_RIGHT_CORNER;
}

/*
Selecting in isometric maps is easy if everything is flat
if it's not, you'll need some tricks
You could use mouse maps to find where on the texture you're clicking
You could find all raised tiles, offset their location, and see if mouse click intersects
    This needs to maintain an actual heightmap, something OpenRCT does I think
You could do this things https://stackoverflow.com/a/49243145 to just make it work
This is why people use engines
*/

void RaiseTerrain(int x, int y, TileMap& heightmap)
{
    // Bounds check
    if (x < 0 || x >= mapwidth || y < 0 || y >= mapheight) return;

    int selectedHeight = heightmap[x][y].height;
    // Clamp height (for now)
    if (selectedHeight >= 1) {
        heightmap[x][y].height = 1;
        return;
    }

    heightmap[x][y].corners = tile_element_raise_styles[4][heightmap[x][y].corners];

    // If terrain is flat, make neighbors slopes
    // Works on flat terrain for now
    if (ControlSettings.MountainTool && ReadTile(heightmap[x][y]).elevation == Tile_Flat) {
        RaiseMountain(x, y, heightmap);
    }
    heightmap[x][y].height += 1;
}
