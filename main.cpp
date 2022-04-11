#include "raylib.h"
#include "tileset.h"
#include "control.h"

int main(void)
{
    InitWindow(screenWidth, screenHeight, "raylib [core] example - input mouse wheel");

    LoadTilesets();
    TraceLog(LOG_INFO, GetWorkingDirectory());

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    Camera2D camera = { 0 };
    camera.target = { 300,300 };
    camera.offset = { screenWidth / 2.0f, screenHeight / 2.0f };
    camera.rotation = 0.f;
    camera.zoom = 1.f;

    // typedef in tileset.h
    // https://mathworld.wolfram.com/GridGraph.html
    TileMap heightmap(mapwidth + 1, std::vector<TileMap::value_type::value_type>(mapheight + 1, { 0 }));

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

        Vector2 mousePos = GetMousePosition();
        mousePos.x += camera.target.x - camera.offset.x;
        mousePos.y += camera.target.y - camera.offset.y;
        // TODO fixing the zoom needs some matrix math I think
        //mousePos.x *= camera.zoom;
        //mousePos.y *= camera.zoom;
        Point selected = CartesianToIso(mousePos.x, mousePos.y);

        for (int i = 0; i < mapwidth; i++) { // x
            for (int j = 0; j < mapheight; j++) { // y
                // Weird offsets because of weird tile with weird space above

                if (i == selected.x && j == selected.y)
                    DrawCursor(selected.x, selected.y, false);
                DrawTile(i, j, heightmap);
                if (i == selected.x && j == selected.y)
                    DrawCursor(selected.x, selected.y, true);
            }
        };

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            RaiseTerrain(selected.x, selected.y, heightmap);
        } else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            // LowerTerrain(heightmap, selected.x, selected.y);
        };

        if (IsKeyPressed(KEY_M)) {
            ControlSettings.MountainTool = !ControlSettings.MountainTool;
        }

        DrawCircle(0, 0, 5, RED);
        DrawCircle(tileWidth / 2, tileHeight / 2, 5, BLUE);
        EndMode2D();

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
        }

        debug = std::string("Press M to toggle Moutain Tool: ");
        debug += std::to_string(ControlSettings.MountainTool);
        DrawText(debug.c_str(), 10, 100, 12, BLACK);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

