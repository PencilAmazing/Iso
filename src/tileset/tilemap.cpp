#include "tilemap.h"

TileMap::TileMap(int x, int y)
    : width(x), length(y)
{
    map = std::vector<std::vector<MapTile>>(x, std::vector<MapTile>(y, { 0 }));
};

uint8_t TileMap::GetNearestCorner(Vector2 mouse, Point tile)
{
    int height = GetTileHeight(tile.x, tile.y);

    Point coords = IsoToCartesian(tile.x, tile.y);
    int x = coords.x + tileWidthHalf;
    int y = coords.y + tileHeightHalf + 8;
    y -= (tileHeightHalf - 8) * height;

    uint8_t select = 0;

    // In top third of tile (from center)
    if (mouse.y <= y - tileHeightHalf / 4) {
        select |= NORTH_CORNER;
    } else if (mouse.y >= y + tileHeightHalf / 4) {
        select |= SOUTH_CORNER;
    } else {
        if (mouse.x <= x - tileWidthHalf / 6) {
            select |= WEST_CORNER;
        } else if (mouse.x >= x + tileWidthHalf / 6) {
            select |= EAST_CORNER;
        } else {
            select |= ALL_CORNERS;
        }
    }

    return select;
};

SelectionStyle TileMap::GetSelectionStyle(Vector2 mouse, Point tile)
{
    uint8_t corner = GetNearestCorner(mouse, tile);

    if ((corner & ALL_CORNERS) == ALL_CORNERS)
        return FullTile;
    else if (corner & NORTH_CORNER)
        return NorthCorner;
    else if (corner & EAST_CORNER)
        return EastCorner;
    else if (corner & SOUTH_CORNER)
        return SouthCorner;
    else if (corner & WEST_CORNER)
        return WestCorner;

    return FullTile;
};

TileDescription TileMap::GetTileDescription(const MapTile* tile)
{
    if (tile)
        return tile_lookup_table[tile->corners & ALL_CORNER_FLAGS];
    return tile_lookup_table[31];
};

TileDescription TileMap::GetTileDescription(Point point)
{
    const MapTile* tile = GetTile(point);
    return GetTileDescription(tile);
};
