#pragma once

#include "tileset.h"

// 0 == floor
// 1 == slope
// 2 == raised
// Give center of tile, raises neighbors accordingly

void RaiseSelection(Point start, Point end, int heightLimit, SelectionStyle style, TileMap& heightmap)
{
    for (int i = start.x; i <= end.x; i++) {
        for (int j = start.y; j <= end.y; j++) {
            if (!IsPointWithinMap(i, j)) continue;
            MapTile& tile = heightmap[i][j];
            if (tile.height > heightLimit) continue;
            tile.corners = tile_element_raise_styles[style][tile.corners];
            //tile.height += 1;
        }
    }
};

void RaiseMountain(int x, int y, int heightLevel, TileMap& heightmap, Point size = { 1,1 })
{
    if (size.x <= 0 || size.y <= 0) return;

    MapTile center = heightmap[x][y];
    //int heightLevel = center.height-1;

    size.x -= 1;
    size.y -= 1;

    // Raise Edges
    RaiseSelection({ x - size.x, y - 1 }, { x + size.x, y - 1 }, heightLevel, SouthEastEdge, heightmap);
    RaiseSelection({ x - 1, y - size.y }, { x - 1, y + size.y }, heightLevel, NorthEastEdge, heightmap);
    RaiseSelection({ x - size.x, y + 1 }, { x + size.x, y + 1 }, heightLevel, NorthWestEdge, heightmap);
    RaiseSelection({ x + 1, y - size.y }, { x + 1, y + size.y }, heightLevel, SouthWestEdge, heightmap);

    // Raise corners
    Point n = { x + 1, y + 1 };
    RaiseSelection(n, n, heightLevel, NorthCorner, heightmap);
    Point w = { x - 1,y + 1 };
    RaiseSelection(w, w, heightLevel, WestCorner, heightmap);
    Point s = { x - 1,y - 1 };
    RaiseSelection(s, s, heightLevel, SouthCorner, heightmap);
    Point e = { x + 1, y - 1 };
    RaiseSelection(e, e, heightLevel, EastCorner, heightmap);
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

// Given a rectangular selection, raises terrain
// size of 0 means single tile
void RaiseTerrain(int x, int y, int size, TileMap& heightmap)
{
    // Bounds check
    if (x < 0 || x >= mapwidth || y < 0 || y >= mapheight) return;
    if (size <= 0) return;

    // Clamp height (for now)
    if (heightmap[x][y].height >= 4) {
        heightmap[x][y].height = 4;
        return;
    }

    size -= 1;
    int max_height = heightmap[x][y].height;
    for (int i = x - size; i <= x + size; i++) {
        for (int j = y - size; j <= y + size; j++) {
            if (IsPointWithinMap(i, j) && heightmap[i][j].height < max_height)
                max_height = heightmap[i][j].height;
        }
    }

    //heightmap[x][y].corners = tile_element_raise_styles[4][heightmap[x][y].corners];
    // Raise initial flat selection
    RaiseSelection({ x - size, y - size }, { x + size, y + size }, max_height, FullTile, heightmap);

    // Smooth edges of selection if enabled
    // Might break if size > 1
    if (ControlSettings.MountainTool && ReadTile(heightmap[x][y]).elevation == Tile_Flat) {
        RaiseMountain(x, y, max_height, heightmap);
        // Smooth the rest
        size += 1;
    }
    // Cleanup, raise tiles with flag set
    for (int i = x - size; i <= x + size; i++) {
        for (int j = y - size; j <= y + size; j++) {
            if (heightmap[i][j].corners & CHANGE_ELEVATION)
                heightmap[i][j].height += 2;
            heightmap[i][j].corners &= ALL_CORNER_FLAGS;
        }
    }
}
