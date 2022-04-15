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

void LoadTerrainSpritesheet()
{
    Image tilemap = LoadImage("assets/tiles_grass.png");
    Image copy;
    // Saddles get special treatement
    TileTexture* textureList[] = { &FloorTile, &CornerSlopeTile, &SlopeTile, &ThreeQuarterSlopeTile, &SteepSlopeTile };

    int yJump = 8 + tileHeight + 16;
    float yOffset = 0;
    for (int j = 0; j < 5; j++) {
        for (int i = 0; i < 4; i++) {
            Rectangle crop = { tileWidth * i, yOffset, tileWidth, tileHeight + 16 };
            copy = ImageFromImage(tilemap, crop);
            textureList[j]->direction[i] = LoadTextureFromImage(copy);
        }
        yOffset += 48;
    }

    Rectangle crop = { 0, yOffset, tileWidth, tileHeight + 16 };
    copy = ImageFromImage(tilemap, crop);
    SaddleSlopeTile.direction[(int)TileDirection::Tile_East] = LoadTextureFromImage(copy);
    SaddleSlopeTile.direction[(int)TileDirection::Tile_West] = LoadTextureFromImage(copy);

    crop = { tileWidth, yOffset, tileWidth, tileHeight + 16 };
    copy = ImageFromImage(tilemap, crop);
    SaddleSlopeTile.direction[(int)TileDirection::Tile_North] = LoadTextureFromImage(copy);
    SaddleSlopeTile.direction[(int)TileDirection::Tile_South] = LoadTextureFromImage(copy);
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
    y -= (tileHeight / 4) * heightmap[i][j].height;
    DrawTexture(texture.direction[(int)tile.direction], x, y, WHITE);
}

void DrawCursor(int i, int j)
{
    Point coords = IsoToCartesian(i, j);
    int x = coords.x;
    int y = coords.y;
    Color cursorColor = (int)GetFrameTime() % 2 == 0 ? WHITE : RED;

    DrawCircle(x + tileWidth, y + tileHeight / 2 + 8, 5, BLUE);
    DrawCircle(x + tileWidth / 2, y + 8, 5, ORANGE);
    DrawCircle(x, y + tileHeight / 2 + 8, 5, RED);
    DrawCircle(x + tileWidth / 2, y + tileHeight + 8, 5, PURPLE);
}
