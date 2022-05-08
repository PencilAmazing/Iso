#pragma once

namespace {
    struct {
        bool MountainTool = true; // Should smooth neighbors
        int Size = 0; // 0 means tile/corner level selection
    } ControlSettings;

    const int screenWidth = 1000;
    const int screenHeight = 900;

    // Terrain generation breaks on non power of 2
    const int mapwidth = 128;
    const int mapheight = 128;

    const int tileWidth = 64;
    const int tileHeight = 32;

    const float cameraSpeed = 30;
};
