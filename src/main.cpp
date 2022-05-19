#include "raylib.h"
#include "raymath.h"

#include "debug.h"
#include "settings.h"
#include "gen/generate.h"
#include "control/control.h"

/*
* TODO:
* Get a terrain spritesheet with actual thickness
* Design a nice UI
* implement actual gameplay
* Maybe terrain generation?
* Separate smoothing functions into edge and corner cases
*/

int main(void)
{
    InitWindow(screenWidth, screenHeight, "raylib [core] example - input mouse wheel");

    LoadTerrainSpritesheet();

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    Camera2D camera = { 0 };
    camera.target = { 300,300 };
    camera.offset = { screenWidth / 2.0f, screenHeight / 2.0f };
    camera.rotation = 0.f;
    camera.zoom = 1.4f;

    // typedef in tileset.h
    // https://mathworld.wolfram.com/GridGraph.html

    //mapheight = mapheight = std::pow(2,6);
    //TileMap heightmap = GenerateTileMap(6);

    //ControlSettings.mapheight = ControlSettings.mapheight = 1024;
    Point dimensions = { 16,16 };
    TileMap heightmap(dimensions.x, std::vector<TileMap::value_type::value_type>(dimensions.y, { 0 }));
    //Point dimensions = { 0,0 };
    //TileMap heightmap = LoadHeightmap(&dimensions);

    // Main game loop
    while (!WindowShouldClose()) {

        if (IsKeyDown(KEY_DOWN))
            camera.target.y += cameraSpeed;
        if (IsKeyDown(KEY_UP))
            camera.target.y -= cameraSpeed;
        if (IsKeyDown(KEY_RIGHT))
            camera.target.x += cameraSpeed;
        if (IsKeyDown(KEY_LEFT))
            camera.target.x -= cameraSpeed;

        BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode2D(camera);

        Vector2 in = GetMousePosition();
        Vector3 mousePos = { in.x, in.y, 0.f };
        mousePos = Vector3Unproject(mousePos, GetCameraMatrix2D(camera), MatrixIdentity());

        Point selected = CartesianToIso(mousePos.x, mousePos.y);

        Point center = CartesianToIso(camera.target.x, camera.target.y);
        int offsetx = (int)(((screenWidth) / tileWidthHalf) / camera.zoom);
        int offsety = (int)(((screenHeight) / tileHeightHalf) / camera.zoom);

        for (int i = std::clamp(center.x - offsetx, 0, dimensions.x); i < std::clamp(center.x + offsetx, 0, dimensions.x); i++) { // x
            for (int j = std::clamp(center.y - offsety, 0, dimensions.y); j < std::clamp(center.y + offsety, 0, dimensions.y); j++) { // y
                DrawTile(i, j, heightmap);
            }
        };
        if (ControlSettings.DrawCursor && IsPointWithinMap(selected.x, selected.y, heightmap))
            DrawCursor({ mousePos.x, mousePos.y }, selected, heightmap[selected.x][selected.y].height);

        int size = 1;
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            RaiseTerrain({ mousePos.x, mousePos.y }, selected, size, heightmap);
        } else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            LowerTerrain({ mousePos.x, mousePos.y }, selected, size, heightmap);
        };

        if (IsKeyPressed(KEY_M)) {
            ControlSettings.MountainTool = !ControlSettings.MountainTool;
        }
        if (IsKeyPressed(KEY_C))
            ControlSettings.DrawCursor = !ControlSettings.DrawCursor;
        //DrawCircle(camera.target.x, camera.target.y, 12, RED);

        camera.zoom += GetMouseWheelMove() * 0.1f;
        camera.zoom = std::clamp(camera.zoom, 0.1f, 2.5f);

        DrawCircle(tileWidthHalf, tileHeightHalf, 5, BLUE);
        EndMode2D();

        WriteDebugToScreen({ mousePos.x, mousePos.y }, selected, heightmap, camera.zoom);

        EndDrawing();
    }

    // Turns out not cleaning up causes glitches later on
    UnloadTerrainSpritesheet();

    CloseWindow();

    return 0;
}

