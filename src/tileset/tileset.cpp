#include "tileset.h"
#include <assert.h>

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

void UnloadTerrainSpritesheet()
{
    UnloadTexture(TerrainSpritesheet);
}

void LoadTerrainSpritesheet()
{
    TerrainSpritesheet = LoadTexture("assets/tiles_grass.png");
    // Saddles get special treatement
    TileTexture* textureList[] = { &FloorTile, &CornerSlopeTile, &SlopeTile, &ThreeQuarterSlopeTile, &SteepSlopeTile };

    int yJump = 8 + tileHeight + 16;
    float yOffset = 0;
    for (int j = 0; j < 5; j++) {
        int xOffset = 0;
        for (int i : {TileDirection::Tile_West, TileDirection::Tile_South, TileDirection::Tile_East, TileDirection::Tile_North}) {
            Rectangle crop = { tileWidth * xOffset, yOffset, tileWidth, tileHeight + 16 };
            //textureList[j]->direction[i] = LoadTextureFromImage(copy);
            textureList[j]->direction[i] = crop;
            xOffset += 1;
        }
        yOffset += 48;
    }

    Rectangle crop = { 0, yOffset, tileWidth, tileHeight + 16 };
    SaddleSlopeTile.direction[(int)TileDirection::Tile_North] = crop;
    SaddleSlopeTile.direction[(int)TileDirection::Tile_South] = crop;

    crop = { tileWidth, yOffset, tileWidth, tileHeight + 16 };
    SaddleSlopeTile.direction[(int)TileDirection::Tile_East] = crop;
    SaddleSlopeTile.direction[(int)TileDirection::Tile_West] = crop;
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
void DrawTile(int i, int j, TileMap& heightmap)
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
        return;
    };

    // Tile set i'm using has really weird margins.
    int height = (int)(heightmap[i][j].height);
    y -= (tileHeight / 4) * height;
    //DrawTexture(texture.direction[(int)tile.direction], x, y, WHITE);
    DrawTextureRec(TerrainSpritesheet, texture.direction[(int)tile.direction], { (float)x, (float)y }, WHITE);
}

uint8_t GetNearestCorner(Vector2 mouse, Point tile, int height)
{
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
}

void DrawCursor(Vector2 mouse, Point tile, int height)
{
    Point coords = IsoToCartesian(tile.x, tile.y);
    int x = coords.x + tileWidthHalf;
    int y = coords.y + tileHeightHalf + 8;
    y -= (tileHeightHalf - 8) * height;

    uint8_t select = GetNearestCorner(mouse, tile, height);
    if (select & NORTH_CORNER)
        DrawCircle(x, y - tileHeightHalf, 5, ORANGE);
    if (select & SOUTH_CORNER)
        DrawCircle(x, y + tileHeightHalf, 5, PURPLE);
    if (select & WEST_CORNER)
        DrawCircle(x - tileWidthHalf, y, 5, RED);
    if (select & EAST_CORNER)
        DrawCircle(x + tileWidthHalf, y, 5, BLUE);
    if (select & ALL_CORNERS)
        DrawCircle(x, y, 5, PINK);
    return;

    // In top third of tile (from center)
    if (mouse.y <= y - tileHeightHalf / 6) {
        DrawCircle(x, y - tileHeightHalf, 5, ORANGE);
    } else if (mouse.y >= y + tileHeightHalf / 6) {
        DrawCircle(x, y + tileHeightHalf, 5, PURPLE);
    } else {
        // Mouse in vertical center
        // Draw left corner
        if (mouse.x <= x - tileWidthHalf / 6) {
            DrawCircle(x - tileWidthHalf, y, 5, RED);
        } else if (mouse.x >= x + tileWidthHalf / 6) {
            DrawCircle(x + tileWidthHalf, y, 5, BLUE);
        } else {
            // Draw full tile
            DrawCircle(x, y, 5, PINK);
            DrawCircle(x + tileWidthHalf, y, 5, BLUE);
            DrawCircle(x - tileWidthHalf, y, 5, RED);
            DrawCircle(x, y - tileHeightHalf, 5, ORANGE);
            DrawCircle(x, y + tileHeightHalf, 5, PURPLE);
        }
    }
}
