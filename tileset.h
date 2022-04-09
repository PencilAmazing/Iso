#pragma once

#include "raylib.h"
#include <string>
#include <vector>

const int screenWidth = 1000;
const int screenHeight = 900;

const int mapwidth = 10;
const int mapheight = 10;

const int tileWidth = 256;
const int tileHeight = 128;

const float cameraSpeed = 30;


enum class TileDirection {
    North = 0,
    East = 1,
    South = 2,
    West = 3
};

struct TileTexture {
    Texture direction[4];
};

struct MapTile {
    int height;
    TileDirection direction;
};

typedef std::vector<std::vector<MapTile>> TileMap;

struct Point {
    int x;
    int y;
};

TileTexture FloorTile;
TileTexture WallTile;
TileTexture PoleTile;
TileTexture CrateTile;
TileTexture SlopeCornerOuterTile;
TileTexture SlopeTile;
TileTexture BlockTile;

Point IsoToCartesian(int i, int j)
{
    return {
        (i - j) * tileWidth / 2 - tileWidth / 2,
        (i + j) * tileHeight / 2 - 3 * tileHeight
    };
};

Point CartesianToIso(float x, float y)
{
    return {
        (int)(y / (float)tileHeight + x / (float)tileWidth),
        (int)(y / (float)tileHeight - x / (float)tileWidth)
    };
}

TileTexture LoadTileVariations(const char* tileName)
{
    std::string prefix("assets/tileset/");
    prefix.append(tileName);
    TileTexture out;
    out.direction[(int)TileDirection::North] = LoadTexture((prefix + std::string("_N.png")).c_str());
    out.direction[(int)TileDirection::South] = LoadTexture((prefix + std::string("_S.png")).c_str());
    out.direction[(int)TileDirection::East] = LoadTexture((prefix + std::string("_E.png")).c_str());
    out.direction[(int)TileDirection::West] = LoadTexture((prefix + std::string("_W.png")).c_str());
    return out;
}

void LoadTilesets()
{
    FloorTile = LoadTileVariations("floor");
    WallTile = LoadTileVariations("wall");
    PoleTile = LoadTileVariations("pole");
    CrateTile = LoadTileVariations("crate");
    SlopeCornerOuterTile = LoadTileVariations("sloperCornerOuter");
    SlopeTile = LoadTileVariations("slope");
    BlockTile = LoadTileVariations("block");
}

// Takes in map coords
void DrawTile(int i, int j, int tileID = 0)
{
    Point coords = IsoToCartesian(i, j);
    int x = coords.x;
    int y = coords.y;

    switch (tileID) {
    case 1: // Draw Cursor
        DrawTexture(PoleTile.direction[(int)TileDirection::North], x, y, WHITE);
        break;
    case 0:
    default:
        DrawTexture(FloorTile.direction[(int)TileDirection::North], x, y, WHITE);
        break;
    }
}

void DrawCursor(int i, int j, bool drawFront)
{
    Point coords = IsoToCartesian(i, j);
    int x = coords.x;
    int y = coords.y;
    Color cursorColor = (int)GetFrameTime() % 2 == 0 ? WHITE : RED;

    if (drawFront) {
        DrawTexture(PoleTile.direction[(int)TileDirection::North], x, y, cursorColor);
        DrawTexture(PoleTile.direction[(int)TileDirection::East], x, y, cursorColor);
    } else { // Draw back
        DrawTexture(PoleTile.direction[(int)TileDirection::South], x, y, cursorColor);
        DrawTexture(PoleTile.direction[(int)TileDirection::West], x, y, cursorColor);
    }
}

void DrawFloor(int i, int j, TileMap heightmap)
{
    Point coords = IsoToCartesian(i, j);
    // Diamond square algorithm would be easy here
    MapTile tile = heightmap[i][j];
    if (tile.height == 0) {
        DrawTexture(FloorTile.direction[(int)TileDirection::North], coords.x, coords.y, WHITE);
    } else if (tile.height == 1) {
        DrawTexture(SlopeTile.direction[(int)tile.direction], coords.x, coords.y, WHITE);
    } else if (tile.height == 2) {
        DrawTexture(BlockTile.direction[(int)TileDirection::North], coords.x, coords.y, WHITE);
    }
}
