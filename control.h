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

    heightmap[i][j + 1].corners = tile_element_raise_styles[5][heightmap[i][j + 1].corners];
    heightmap[i + 1][j].corners = tile_element_raise_styles[6][heightmap[i + 1][j].corners];
    heightmap[i][j - 1].corners = tile_element_raise_styles[7][heightmap[i][j - 1].corners];
    heightmap[i - 1][j].corners = tile_element_raise_styles[8][heightmap[i - 1][j].corners];

    heightmap[i - 1][j - 1].corners = tile_element_raise_styles[1][heightmap[i - 1][j - 1].corners];
    heightmap[i - 1][j + 1].corners = tile_element_raise_styles[2][heightmap[i - 1][j + 1].corners];
    heightmap[i + 1][j + 1].corners = tile_element_raise_styles[3][heightmap[i + 1][j + 1].corners];
    heightmap[i + 1][j - 1].corners = tile_element_raise_styles[0][heightmap[i + 1][j - 1].corners];

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
    if (selectedHeight >= 3) {
        heightmap[x][y].height = 3;
        return;
    }

    heightmap[x][y].corners = tile_element_raise_styles[4][heightmap[x][y].corners];

    // If terrain is flat, make neighbors slopes
    // Works on flat terrain for now
    // TODO: Only affect neigboring tiles of similar height
    if (ControlSettings.MountainTool && ReadTile(heightmap[x][y]).elevation == Tile_Flat) {
        RaiseMountain(x, y, heightmap);
    }

    for (int i = x - 1; i <= x + 1; i++) {
        for (int j = y - 1; j <= y + 1; j++) {
            if (heightmap[i][j].corners & CHANGE_ELEVATION) {
                heightmap[i][j].height += 1;
            }
            heightmap[i][j].corners &= ALL_CORNER_FLAGS;
        }
    }
}
