#pragma once

#include "raylib.h"

#include "../tileset.h"
#include <cmath>
#include <algorithm>
#include <array>
#include <vector>
#include <string>

// Generates a noise image using Diamond Square algorithm
// Image is a square of size 2^n + 1
// 1->3, 2->5, 3->9, 4->17, 5->33
std::vector<std::vector<short>> GenerateHeightMap(int n);

TileMap GenerateTileMap(int n);