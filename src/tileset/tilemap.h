#pragma once

#include <vector>
#include "tileset.h"

typedef std::vector<std::vector<MapTile>> TileMapArray;

class TileMap {
private:
    TileMapArray map;
    // width
    int width;
    // length
    int length;

public:
    TileMap() = default;
    TileMap(int x, int y);

    std::vector<MapTile>& operator[](int i)
    {
        return map[i];
    }

    const int Width()
    {
        return width;
    }
    const int Length()
    {
        return length;
    }

    const TileMapArray& GetTileMap()
    {
        return map;
    }

    bool IsPointWithinMap(int x, int y)
    {
        return x >= 0 && y >= 0 && x < width && y < length;
    }

    bool IsPointWithinMap(Point point)
    {
        return IsPointWithinMap(point.x, point.y);
    }

    const MapTile* GetTile(int x, int y)
    {
        if (IsPointWithinMap(x, y))
            return &map[x][y];
        return nullptr;
    }

    const MapTile* GetTile(Point point)
    {
        return GetTile(point.x, point.y);
    }

    int GetTileHeight(int x, int y)
    {
        if (IsPointWithinMap(x, y))
            return map[x][y].height;
        return -1;
    }

    // Return bitflag corresponding to corner closest to cursor
    uint8_t GetNearestCorner(Vector2 mouse, Point tile);
    // Return selection style corresponding to corner closest to cursor
    SelectionStyle GetSelectionStyle(Vector2 mouse, Point tile);

    TileDescription GetTileDescription(Point point);
    TileDescription GetTileDescription(const MapTile* tile);
};
