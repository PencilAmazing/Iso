#pragma once

#include "raylib.h"
#include <string>
#include <vector>
#include <assert.h>

const int screenWidth = 1000;
const int screenHeight = 900;

const int mapwidth = 10;
const int mapheight = 10;

const int tileWidth = 256;
const int tileHeight = 128;

const float cameraSpeed = 30;

enum class TileDirection {
    North,
    East,
    South,
    West
};

enum class TileElevation {
    Flat, // Flat, 
    CornerSlope, // One corner is raised
    Slope, // Two adjacent corners are raised
    SteepSlope, // One corner is raised with a steep bit set
    Invalid // Just in case
};

struct TileDescription {
    TileDirection direction;
    TileElevation elevation;
};

struct TileTexture {
    Texture direction[4];
};

/*
.............*.... top right
top left *........* bottom right
.............*.... bottom left

I cannot be bother to rename this now
*/

enum TileCorner {
    TOP_RIGHT_CORNER = 1 << 0,
    BOTTOM_RIGHT_CORNER = 1 << 1,
    BOTTOM_LEFT_CORNER = 1 << 2,
    TOP_LEFT_CORNER = 1 << 3,
    STEEP_CORNER = 1 << 4 // Valid if one corner is raised
};

struct MapTile {
    uint8_t corners;
    int height;
};

const uint8_t CornerLookupTable[] = {
    // Flat
    0,
    // Corner slope
    TOP_RIGHT_CORNER,
    BOTTOM_RIGHT_CORNER,
    BOTTOM_LEFT_CORNER,
    TOP_LEFT_CORNER,
    //  Slopes
    TOP_RIGHT_CORNER | TOP_LEFT_CORNER,
    TOP_RIGHT_CORNER | BOTTOM_RIGHT_CORNER,
    BOTTOM_LEFT_CORNER | BOTTOM_RIGHT_CORNER,
    TOP_LEFT_CORNER | BOTTOM_LEFT_CORNER,
    // Steep Slope
    STEEP_CORNER | TOP_RIGHT_CORNER,
    STEEP_CORNER | BOTTOM_RIGHT_CORNER,
    STEEP_CORNER | BOTTOM_LEFT_CORNER,
    STEEP_CORNER | TOP_LEFT_CORNER,
    // Anything not in this table is invalid
};

const TileDirection DirectionLookupTable[] = {
    // Flat
    TileDirection::North,
    // Corner Slope
    TileDirection::North,
    TileDirection::West,
    TileDirection::South,
    TileDirection::East,
    // Slopes
    TileDirection::South,
    TileDirection::East,
    TileDirection::North,
    TileDirection::West,
    // Steep Slope
    TileDirection::North,
    TileDirection::West,
    TileDirection::South,
    TileDirection::East
};

const TileElevation ElevationLookupTable[] = {
    TileElevation::Flat,
    TileElevation::CornerSlope,
    TileElevation::CornerSlope,
    TileElevation::CornerSlope,
    TileElevation::CornerSlope,
    TileElevation::Slope,
    TileElevation::Slope,
    TileElevation::Slope,
    TileElevation::Slope,
    TileElevation::SteepSlope,
    TileElevation::SteepSlope,
    TileElevation::SteepSlope,
    TileElevation::SteepSlope
};

inline bool TestCorner(MapTile tile, TileCorner corner)
{
    return (tile.corners & corner) == corner;
}

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

TileDescription ReadTile(MapTile tile)
{
    //MapTile tile = heightmap[i][j];
    int index = -1;
    for (int i = 0; i < sizeof(CornerLookupTable); i++) {
        if (CornerLookupTable[i] == tile.corners) {
            index = i;
            break;
        }
    }

    TileDescription out;
    out.direction = TileDirection::North;
    out.elevation = TileElevation::Invalid;
    if (index != -1) {
        out.direction = DirectionLookupTable[index];
        out.elevation = ElevationLookupTable[index];
    }

    return out;
}

// Takes in map coords
void DrawTile(int i, int j, TileMap heightmap)
{
    Point coords = IsoToCartesian(i, j);
    int x = coords.x;
    int y = coords.y;
    //assert(x >= 0 && y >= 0);
    
    TileDescription tile = ReadTile(heightmap[i][j]);
    TileTexture texture;

    switch (tile.elevation) {
    case TileElevation::Flat:
        texture = FloorTile;
        break;
    case TileElevation::Slope:
        texture = SlopeTile;
        break;
    case TileElevation::CornerSlope:
        texture = SlopeCornerOuterTile;
        break;
    case TileElevation::SteepSlope:
        assert(false, "Whoop de fucking do you're missing a texture");
        break;
    case TileElevation::Invalid:
    default:
        assert(false, "Invalid I guess");
        break;
    };

    y -= tileHeight * heightmap[i][j].height;
    DrawTexture(texture.direction[(int)tile.direction], x, y, WHITE);
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
