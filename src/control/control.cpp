#include "control.h"

void RaiseSelection(Point start, Point end, int heightLimit, SelectionStyle style, TileMap& heightmap)
{
    for (int i = start.x; i <= end.x; i++) {
        for (int j = start.y; j <= end.y; j++) {
            if (!IsPointWithinMap(i, j, heightmap)) continue;
            MapTile& tile = heightmap[i][j];
            if (tile.height > heightLimit) continue;
            tile.corners = tile_element_raise_styles[style][tile.corners];
            //tile.height += 1;
        }
    }
};

void LowerSelection(Point start, Point end, int heightLimit, SelectionStyle style, TileMap& heightmap)
{
    for (int i = start.x; i <= end.x; i++) {
        for (int j = start.y; j <= end.y; j++) {
            if (!IsPointWithinMap(i, j, heightmap)) continue;
            MapTile& tile = heightmap[i][j];
            if (tile.height > heightLimit) continue;
            tile.corners = tile_element_lower_styles[style][tile.corners];
            //tile.height += 1;
        }
    }
};

void RaiseMountain(int x, int y, int heightLevel, TileMap& heightmap, Point size)
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
    RaiseSelection(w, w, heightLevel, EastCorner, heightmap);
    Point s = { x - 1,y - 1 };
    RaiseSelection(s, s, heightLevel, SouthCorner, heightmap);
    Point e = { x + 1, y - 1 };
    RaiseSelection(e, e, heightLevel, WestCorner, heightmap);
};

void RaiseTerrain(Vector2 mouse, Point selected, int size, TileMap& heightmap)
{
    int x = selected.x;
    int y = selected.y;

    // Bounds check
    if (x < 0 || x >= mapwidth || y < 0 || y >= mapheight) return;
    if (size <= 0) return;

    uint8_t select = GetNearestCorner(mouse, selected, heightmap[x][y].height);

    size -= 1;
    int max_height = heightmap[x][y].height;
    for (int i = x - size; i <= x + size; i++) {
        for (int j = y - size; j <= y + size; j++) {
            if (IsPointWithinMap(i, j, heightmap) && heightmap[i][j].height < max_height)
                max_height = heightmap[i][j].height;
        }
    }

    //heightmap[x][y].corners = tile_element_raise_styles[4][heightmap[x][y].corners];
    // Raise initial flat selection
    SelectionStyle style = FullTile;
    if ((select & ALL_CORNERS) == ALL_CORNERS)
        style = FullTile;
    else if (select & NORTH_CORNER)
        style = NorthCorner;
    else if (select & EAST_CORNER)
        style = EastCorner;
    else if (select & SOUTH_CORNER)
        style = SouthCorner;
    else if (select & WEST_CORNER)
        style = WestCorner;

    RaiseSelection({ x - size, y - size }, { x + size, y + size }, max_height, style, heightmap);

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
                heightmap[i][j].height += 1;
            heightmap[i][j].corners &= ALL_CORNER_FLAGS;
        }
    }
}

void LowerTerrain(Vector2 mouse, Point selected, int size, TileMap& heightmap)
{
    int x = selected.x;
    int y = selected.y;

    // Bounds check
    if (x < 0 || x >= mapwidth || y < 0 || y >= mapheight) return;
    if (size <= 0) return;

    uint8_t select = GetNearestCorner(mouse, selected, heightmap[x][y].height);

    size -= 1;
    int max_height = heightmap[x][y].height;
    for (int i = x - size; i <= x + size; i++) {
        for (int j = y - size; j <= y + size; j++) {
            if (IsPointWithinMap(i, j, heightmap) && heightmap[i][j].height < max_height)
                max_height = heightmap[i][j].height;
        }
    }

    //heightmap[x][y].corners = tile_element_raise_styles[4][heightmap[x][y].corners];
    // Raise initial flat selection
    SelectionStyle style = FullTile;
    if ((select & ALL_CORNERS) == ALL_CORNERS)
        style = FullTile;
    else if (select & NORTH_CORNER)
        style = NorthCorner;
    else if (select & EAST_CORNER)
        style = EastCorner;
    else if (select & SOUTH_CORNER)
        style = SouthCorner;
    else if (select & WEST_CORNER)
        style = WestCorner;

    LowerSelection({ x - size, y - size }, { x + size, y + size }, max_height, style, heightmap);

    // Cleanup, raise tiles with flag set
    for (int i = x - size; i <= x + size; i++) {
        for (int j = y - size; j <= y + size; j++) {
            if (heightmap[i][j].corners & CHANGE_ELEVATION)
                heightmap[i][j].height -= 1;
            heightmap[i][j].corners &= ALL_CORNER_FLAGS;
        }
    }
}
