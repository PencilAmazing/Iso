#pragma once

#include "raylib.h"

#include "tileset/tileset.h"
#include <string>

std::string BinaryToString(uint8_t binary)
{
    std::string out;
    out.resize(8, '0');
    for (int i = 0; i < 8; i++)
        if (binary & (1 << i))
            out[7 - i] = '1';
    return out;
}

void WriteDebugToScreen(Vector2 mousePos, Point selected, const TileMap& heightmap)
{
    std::string debug = std::string("Mouse pos at ");
    debug += std::to_string(mousePos.x);
    debug += ' ';
    debug += std::to_string(mousePos.y);
    DrawText(debug.c_str(), 10, 10, 12, BLACK);

    debug = std::string("Mouse Iso at ");
    debug += std::to_string(selected.x);
    debug += ' ';
    debug += std::to_string(selected.y);
    DrawText(debug.c_str(), 10, 40, 12, BLACK);

    if (selected.x >= 0 && selected.x < mapwidth && selected.y >= 0 && selected.y < mapheight) {
        debug = std::string("Map height ");
        debug += std::to_string(heightmap[selected.x][selected.y].height);
        DrawText(debug.c_str(), 10, 70, 12, BLACK);

        debug = BinaryToString(heightmap[selected.x][selected.y].corners);
        DrawText(debug.c_str(), screenWidth - 100, 30, 16, BLACK);
        TileDescription desc = ReadTile(heightmap[selected.x][selected.y]);
        debug = std::string("Tile description ");
        debug += std::to_string(desc.direction);
        debug += ' ';
        debug += std::to_string(desc.elevation);
        DrawText(debug.c_str(), screenWidth - 200, 60, 12, BLACK);
    }

    debug = std::string("Press M to toggle Moutain Tool: ");
    debug += std::to_string(ControlSettings.MountainTool);
    DrawText(debug.c_str(), 10, 100, 12, BLACK);
};