#pragma once

namespace {
    struct {
        bool MountainTool = true; // Should smooth neighbors
        int Size = 0; // 0 means tile/corner level selection
    } ControlSettings;

    const int screenWidth = 1000;
    const int screenHeight = 900;

    const int mapwidth = 10;
    const int mapheight = 10;

    const int tileWidth = 64;
    const int tileHeight = 32;

    const float cameraSpeed = 30;
};
