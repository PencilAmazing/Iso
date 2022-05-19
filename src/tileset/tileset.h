#pragma once

#include "raylib.h"

#include "../settings.h"
#include <vector>


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
    Tile_East,
    Tile_South,
    Tile_West,
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
    //Texture2D direction[4];
    Rectangle direction[4];
};

enum TileCorner : uint8_t {
    NORTH_CORNER = 1 << 0,
    EAST_CORNER = 1 << 1,
    SOUTH_CORNER = 1 << 2,
    WEST_CORNER = 1 << 3,
    STEEP_CORNER = 1 << 4, // Valid if three corners are raised
    CHANGE_ELEVATION = 1 << 5 // Set when tile changes elevation
};

enum SelectionStyle {
    NorthCorner,
    EastCorner,
    SouthCorner,
    WestCorner,
    FullTile,
    SouthEastEdge,
    SouthWestEdge,
    NorthWestEdge,
    NorthEastEdge
};

const uint8_t ALL_CORNERS = NORTH_CORNER | EAST_CORNER | SOUTH_CORNER | WEST_CORNER;
const uint8_t ALL_CORNER_FLAGS = ALL_CORNERS | STEEP_CORNER;

struct MapTile {
    uint8_t corners;
    uint8_t height;
};

// Generated by script/GenerateLookupTable.py
const TileDescription tile_lookup_table[] = {
    {Tile_Flat, Tile_North },
    {Tile_Corner, Tile_North },
    {Tile_Corner, Tile_East },
    {Tile_Slope, Tile_North },
    {Tile_Corner, Tile_South },
    {Tile_Saddle, Tile_North },
    {Tile_Slope, Tile_East },
    {Tile_ThreeQuarter, Tile_East },
    {Tile_Corner, Tile_West },
    {Tile_Slope, Tile_West },
    {Tile_Saddle, Tile_East },
    {Tile_ThreeQuarter, Tile_North },
    {Tile_Slope, Tile_South },
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

typedef std::vector<std::vector<MapTile>> TileMap;

struct Point {
    int x;
    int y;

    Point operator+(const Point& other) const
    {
        return { x + other.x, y + other.y };
    }
    Point operator-(const Point& other) const
    {
        return { x - other.x, y - other.y };
    }
};

namespace {
    Texture TerrainSpritesheet;

    TileTexture FloorTile;
    TileTexture CornerSlopeTile;
    TileTexture SlopeTile;
    TileTexture ThreeQuarterSlopeTile;
    TileTexture SteepSlopeTile;
    TileTexture SaddleSlopeTile;
};

Point IsoToCartesian(int i, int j);
Point CartesianToIso(float x, float y);

inline bool IsPointWithinMap(int i, int j, const TileMap& map)
{
    return i >= 0 && j >= 0 && i < map.size() && j < map[0].size();
};

inline uint8_t GetNearestCorner(Vector2 mouse, Point tile, int height);

void LoadTerrainSpritesheet();
void UnloadTerrainSpritesheet();

TileDescription ReadTile(MapTile tile);

// Takes in map coords
void DrawTile(int i, int j, TileMap& heightmap);
void DrawCursor(Vector2 mouse, Point tile, int height);
