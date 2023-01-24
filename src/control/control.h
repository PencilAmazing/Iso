#pragma once

#include "../tileset/tilemap.h"

void RaiseSelection(Point start, Point end, int heightLimit, SelectionStyle style, TileMap& heightmap);
void LowerSelection(Point start, Point end, int heightLimit, SelectionStyle style, TileMap& heightmap);

void RaiseMountain(int x, int y, int heightLevel, TileMap& heightmap, Point size = { 1,1 });

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
void RaiseTerrain(Vector2 mouse, Point selected, int size, TileMap& heightmap);
void LowerTerrain(Vector2 mouse, Point selected, int size, TileMap& heightmap);
