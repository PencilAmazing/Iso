#pragma once

#include "raylib.h"
#include <string>
#include <vector>
#include <assert.h>

const int screenWidth = 1000;
const int screenHeight = 900;

const int mapwidth = 10;
const int mapheight = 10;

const int tileWidth = 64;
const int tileHeight = 32;

const float cameraSpeed = 30;

// Following openttd's convention
// https://wiki.openttd.org/en/Development/Directions
/*
-----N-----
--NW---NE--
W----0----E
--SW---SE--
-----S-----
*/
enum TileDirection {
    Tile_North,
    //Tile_NorthEast,
    Tile_East,
    //Tile_SouthEast,
    Tile_South,
    //Tile_SouthWest,
    Tile_West,
    //Tile_NorthWest
};

enum TileElevation {
    Tile_Flat, // Flat, 
    Tile_Corner, // One corner is raised
    Tile_Slope, // Two adjacent corners are raised
    Tile_Saddle, // Two opposing corners are raised
    Tile_ThreeQuarter, // Three corners are raised
    Tile_Steep, // Three corners is raised with a steep bit set

    Tile_Invalid // Just in case
};

struct TileDescription {
    TileElevation elevation;
    TileDirection direction;
};

struct TileTexture {
    Texture direction[4];
};

enum TileCorner : uint8_t {
    NORTH_CORNER = 1 << 0,
    EAST_CORNER = 1 << 1,
    SOUTH_CORNER = 1 << 2,
    WEST_CORNER = 1 << 3,
    STEEP_CORNER = 1 << 4 // Valid if one corner is raised
};

const uint8_t ALL_CORNERS = NORTH_CORNER | EAST_CORNER | SOUTH_CORNER | WEST_CORNER;
const uint8_t ALL_CORNER_FLAGS = ALL_CORNERS | STEEP_CORNER;

struct MapTile {
    uint8_t corners;
    int height;
};

// Write a script for this shit im not typing all of that
const TileDescription tile_lookup_table[] = {
    {Tile_Flat, Tile_North },
    {Tile_Corner, Tile_North },
    {Tile_Corner, Tile_East },
    {Tile_Slope, Tile_East },
    {Tile_Corner, Tile_South },
    {Tile_Saddle, Tile_North },
    {Tile_Slope, Tile_South },
    {Tile_ThreeQuarter, Tile_East },
    {Tile_Corner, Tile_West },
    {Tile_Slope, Tile_North },
    {Tile_Saddle, Tile_East },
    {Tile_ThreeQuarter, Tile_North },
    {Tile_Slope, Tile_West },
    {Tile_ThreeQuarter, Tile_West },
    {Tile_ThreeQuarter, Tile_South },
    {Tile_Invalid, Tile_North },
    {Tile_Corner, Tile_North },
    {Tile_Slope, Tile_North },
    {Tile_Slope, Tile_North },
    {Tile_Invalid, Tile_North },
    {Tile_Slope, Tile_North },
    {Tile_Invalid, Tile_North },
    {Tile_Invalid, Tile_North },
    {Tile_Steep, Tile_East },
    {Tile_Slope, Tile_North },
    {Tile_Invalid, Tile_North },
    {Tile_Invalid, Tile_North },
    {Tile_Steep, Tile_North },
    {Tile_Invalid, Tile_North },
    {Tile_Steep, Tile_West },
    {Tile_Steep, Tile_South },
    {Tile_Invalid, Tile_North },
};

// Yes I stole it from openrct
// 0x00981A1E
// Table of pre-calculated surface slopes (32) when raising the land tile for a given selection (5)
// 0x1F = new slope
// 0x20 = base height increases
const uint8_t tile_element_raise_styles[9][32] = {
    {
        0x01, 0x1B, 0x03, 0x1B, 0x05, 0x21, 0x07, 0x21, 0x09, 0x1B, 0x0B, 0x1B, 0x0D, 0x21, 0x20, 0x0F,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x23, 0x18, 0x19, 0x1A, 0x3B, 0x1C, 0x29, 0x24, 0x1F,
    }, // MAP_SELECT_TYPE_CORNER_0
       // (absolute rotation)
    {
        0x02, 0x03, 0x17, 0x17, 0x06, 0x07, 0x17, 0x17, 0x0A, 0x0B, 0x22, 0x22, 0x0E, 0x20, 0x22, 0x0F,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x37, 0x18, 0x19, 0x1A, 0x23, 0x1C, 0x28, 0x26, 0x1F,
    }, // MAP_SELECT_TYPE_CORNER_1
    {
        0x04, 0x05, 0x06, 0x07, 0x1E, 0x24, 0x1E, 0x24, 0x0C, 0x0D, 0x0E, 0x20, 0x1E, 0x24, 0x1E, 0x0F,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x26, 0x18, 0x19, 0x1A, 0x21, 0x1C, 0x2C, 0x3E, 0x1F,
    }, // MAP_SELECT_TYPE_CORNER_2
    {
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x20, 0x1D, 0x1D, 0x28, 0x28, 0x1D, 0x1D, 0x28, 0x0F,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x22, 0x18, 0x19, 0x1A, 0x29, 0x1C, 0x3D, 0x2C, 0x1F,
    }, // MAP_SELECT_TYPE_CORNER_3
    {
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
        0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x20, 0x20, 0x20, 0x21, 0x20, 0x28, 0x24, 0x20,
    }, // MAP_SELECT_TYPE_FULL
    {
        0x0C, 0x0D, 0x0E, 0x20, 0x0C, 0x0D, 0x0E, 0x20, 0x0C, 0x0D, 0x0E, 0x20, 0x2C, 0x2C, 0x2C, 0x2C,
        0x0C, 0x0D, 0x0E, 0x20, 0x0C, 0x0C, 0x0E, 0x22, 0x0C, 0x0D, 0x0E, 0x21, 0x2C, 0x2C, 0x2C, 0x2C,
    }, // MAP_SELECT_TYPE_EDGE_0
    {
        0x09, 0x09, 0x0B, 0x0B, 0x0D, 0x0D, 0x20, 0x20, 0x09, 0x29, 0x0B, 0x29, 0x0D, 0x29, 0x20, 0x29,
        0x09, 0x09, 0x0B, 0x0B, 0x0D, 0x0D, 0x24, 0x22, 0x09, 0x29, 0x0B, 0x29, 0x0D, 0x29, 0x24, 0x29,
    }, // MAP_SELECT_TYPE_EDGE_1
    {
        0x03, 0x03, 0x03, 0x23, 0x07, 0x07, 0x07, 0x23, 0x0B, 0x0B, 0x0B, 0x23, 0x20, 0x20, 0x20, 0x23,
        0x03, 0x03, 0x03, 0x23, 0x07, 0x07, 0x07, 0x23, 0x0B, 0x0B, 0x0B, 0x23, 0x20, 0x28, 0x24, 0x23,
    }, // MAP_SELECT_TYPE_EDGE_2
    {
        0x06, 0x07, 0x06, 0x07, 0x06, 0x07, 0x26, 0x26, 0x0E, 0x20, 0x0E, 0x20, 0x0E, 0x20, 0x26, 0x26,
        0x06, 0x07, 0x06, 0x07, 0x06, 0x07, 0x26, 0x26, 0x0E, 0x20, 0x0E, 0x21, 0x0E, 0x28, 0x26, 0x26,
    }, // MAP_SELECT_TYPE_EDGE_3
};

// 0x00981ABE
// Basically the inverse of the table above.
// 0x1F = new slope
// 0x20 = base height increases
const uint8_t tile_element_lower_styles[9][32] = {
    {
        0x2E, 0x00, 0x2E, 0x02, 0x3E, 0x04, 0x3E, 0x06, 0x2E, 0x08, 0x2E, 0x0A, 0x3E, 0x0C, 0x3E, 0x0F,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x06, 0x18, 0x19, 0x1A, 0x0B, 0x1C, 0x0C, 0x3E, 0x1F,
    }, // MAP_SELECT_TYPE_CORNER_0
    {
        0x2D, 0x2D, 0x00, 0x01, 0x2D, 0x2D, 0x04, 0x05, 0x3D, 0x3D, 0x08, 0x09, 0x3D, 0x3D, 0x0C, 0x0F,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x07, 0x18, 0x19, 0x1A, 0x09, 0x1C, 0x3D, 0x0C, 0x1F,
    }, // MAP_SELECT_TYPE_CORNER_1
    {
        0x2B, 0x3B, 0x2B, 0x3B, 0x00, 0x01, 0x02, 0x03, 0x2B, 0x3B, 0x2B, 0x3B, 0x08, 0x09, 0x0A, 0x0F,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x03, 0x18, 0x19, 0x1A, 0x3B, 0x1C, 0x09, 0x0E, 0x1F,
    }, // MAP_SELECT_TYPE_CORNER_2
    {
        0x27, 0x27, 0x37, 0x37, 0x27, 0x27, 0x37, 0x37, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x0F,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x37, 0x18, 0x19, 0x1A, 0x03, 0x1C, 0x0D, 0x06, 0x1F,
    }, // MAP_SELECT_TYPE_CORNER_3
    {
        0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x0D, 0x0E, 0x00,
    }, // MAP_SELECT_TYPE_FULL
    {
        0x23, 0x23, 0x23, 0x23, 0x00, 0x01, 0x02, 0x03, 0x00, 0x01, 0x02, 0x03, 0x00, 0x01, 0x02, 0x03,
        0x23, 0x23, 0x23, 0x23, 0x00, 0x01, 0x02, 0x03, 0x00, 0x01, 0x02, 0x03, 0x00, 0x0D, 0x0E, 0x03,
    }, // MAP_SELECT_TYPE_EDGE_0
    {
        0x26, 0x00, 0x26, 0x02, 0x26, 0x04, 0x26, 0x06, 0x00, 0x00, 0x02, 0x02, 0x04, 0x04, 0x06, 0x06,
        0x26, 0x00, 0x26, 0x02, 0x26, 0x04, 0x26, 0x06, 0x00, 0x00, 0x02, 0x0B, 0x04, 0x0D, 0x06, 0x06,
    }, // MAP_SELECT_TYPE_EDGE_1
    {
        0x2C, 0x00, 0x00, 0x00, 0x2C, 0x04, 0x04, 0x04, 0x2C, 0x08, 0x08, 0x08, 0x2C, 0x0C, 0x0C, 0x0C,
        0x2C, 0x00, 0x00, 0x00, 0x2C, 0x04, 0x04, 0x07, 0x2C, 0x08, 0x08, 0x0B, 0x2C, 0x0C, 0x0C, 0x0C,
    }, // MAP_SELECT_TYPE_EDGE_2
    {
        0x29, 0x29, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x29, 0x29, 0x08, 0x09, 0x08, 0x09, 0x08, 0x09,
        0x29, 0x29, 0x00, 0x01, 0x00, 0x01, 0x00, 0x07, 0x29, 0x29, 0x08, 0x09, 0x08, 0x09, 0x0E, 0x09,
    }, // MAP_SELECT_TYPE_EDGE_3
};

inline bool TestCorner(MapTile tile, TileCorner corner)
{
    return (tile.corners & corner) == corner;
};

typedef std::vector<std::vector<MapTile>> TileMap;

struct Point {
    int x;
    int y;
};

//TileTexture WallTile;
//TileTexture PoleTile;
//TileTexture CrateTile;

TileTexture FloorTile;
TileTexture CornerSlopeTile;
TileTexture SlopeTile;
TileTexture ThreeQuarterSlopeTile;
TileTexture SteepSlopeTile;
TileTexture SaddleSlopeTile;

Point IsoToCartesian(int i, int j)
{
    return {
        (i - j) * tileWidth / 2 - tileWidth / 2,
        (i + j) * tileHeight / 2 - 8
    };
};

Point CartesianToIso(float x, float y)
{
    return {
        (int)(y / (float)tileHeight + x / (float)tileWidth),
        (int)(y / (float)tileHeight - x / (float)tileWidth)
    };
}

//TileTexture LoadTileVariations(const char* tileName)
//{
//    std::string prefix("assets/tileset/");
//    prefix.append(tileName);
//    TileTexture out;
//    out.direction[(int)Tile_North] = LoadTexture((prefix + std::string("_N.png")).c_str());
//    out.direction[(int)Tile_South] = LoadTexture((prefix + std::string("_S.png")).c_str());
//    out.direction[(int)Tile_East] = LoadTexture((prefix + std::string("_E.png")).c_str());
//    out.direction[(int)Tile_West] = LoadTexture((prefix + std::string("_W.png")).c_str());
//    return out;
//}
//void LoadTilesets()
//{
//    FloorTile = LoadTileVariations("floor");
//    WallTile = LoadTileVariations("wall");
//    PoleTile = LoadTileVariations("pole");
//    CrateTile = LoadTileVariations("crate");
//    SlopeCornerOuterTile = LoadTileVariations("sloperCornerOuter");
//    SlopeTile = LoadTileVariations("slope");
//    BlockTile = LoadTileVariations("block");
//}

void LoadTerrainSpritesheet()
{
    Image tilemap = LoadImage("assets/tiles_grass.png");
    Image copy;
    // Saddles get special treatement
    TileTexture* textureList[] = { &FloorTile, &CornerSlopeTile, &SlopeTile, &ThreeQuarterSlopeTile, &SteepSlopeTile };

    int yJump = 8 + tileHeight+16;
    float yOffset = 0;
    for (int j = 0; j < 5; j++) {
        for (int i = 0; i < 4; i++) {
            Rectangle crop = { tileWidth * i, yOffset, tileWidth, tileHeight+16 };
            copy = ImageFromImage(tilemap, crop);
            textureList[j]->direction[i] = LoadTextureFromImage(copy);
        }
        yOffset += 48;
    }

    Rectangle crop = { 0, yOffset, tileWidth, tileHeight };
    copy = ImageFromImage(tilemap, crop);
    SaddleSlopeTile.direction[(int)TileDirection::Tile_North] = LoadTextureFromImage(copy);
    SaddleSlopeTile.direction[(int)TileDirection::Tile_South] = LoadTextureFromImage(copy);

    crop = { tileWidth, yOffset, tileWidth, tileHeight };
    copy = ImageFromImage(tilemap, crop);
    SaddleSlopeTile.direction[(int)TileDirection::Tile_East] = LoadTextureFromImage(copy);
    SaddleSlopeTile.direction[(int)TileDirection::Tile_West] = LoadTextureFromImage(copy);
}

TileDescription ReadTile(MapTile tile)
{
    // If you break it you get no pizza
    return tile_lookup_table[tile.corners & ALL_CORNER_FLAGS];

    //if (tile.corners <= ALL_CORNER_FLAGS)
    //    return tile_lookup_table[tile.corners];
    //else
    //    return { Tile_North, Tile_Invalid };
};

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
    case Tile_Flat:
        texture = FloorTile;
        break;
    case Tile_Slope:
        texture = SlopeTile;
        break;
    case Tile_Corner:
        texture = CornerSlopeTile;
        break;
    case Tile_Steep:
        texture = SteepSlopeTile;
        break;
    case Tile_ThreeQuarter:
        texture = ThreeQuarterSlopeTile;
        break;
    case Tile_Saddle:
        texture = SaddleSlopeTile;
        break;
    case Tile_Invalid:
    default:
        assert("You broke it");
        //assert(false, "Invalid I guess");
        //texture = CrateTile;
        break;
    };

    // Tile set i'm using has really weird margins.
    y -= (tileHeight/4) * heightmap[i][j].height;
    DrawTexture(texture.direction[(int)tile.direction], x, y, WHITE);
}

void DrawCursor(int i, int j, bool drawFront)
{
    Point coords = IsoToCartesian(i, j);
    int x = coords.x;
    int y = coords.y;
    Color cursorColor = (int)GetFrameTime() % 2 == 0 ? WHITE : RED;

    if (drawFront) {
        //DrawTexture(PoleTile.direction[(int)Tile_North], x, y, cursorColor);
        //DrawTexture(PoleTile.direction[(int)Tile_East], x, y, cursorColor);

    } else { // Draw back
        DrawTexture(PoleTile.direction[(int)Tile_South], x, y, cursorColor);
        DrawTexture(PoleTile.direction[(int)Tile_West], x, y, cursorColor);
    }
}
