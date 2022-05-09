#include "generate.h"

// keep this above zero
int randmin = 0;
int randmax = 3;

int access(std::vector<std::vector<int>>* array, int side, int x, int y)
{
    //could be replaced with clearing the sign bit but eh
    //int i = abs(x) % (side + 1);
    //int j = abs(y) % (side + 1);
    // this assumes side is a power of two
    int i = x & side;
    int j = y & side;
    return (*array)[i][j];
};

void set(std::vector<std::vector<int>>* array, int x, int y, int value)
{
    (*array)[x][y] = value;
};

// i,j is top left corner of a square
void diamond(std::vector<std::vector<int>>* generation, int side, int x, int y, int stepSize)
{
    int halfSize = stepSize / 2;

    // variable names are relative to center of the diamond
    int bl = access(generation, side, x, y + stepSize);
    int tl = access(generation, side, x, y);
    int tr = access(generation, side, x + stepSize, y);
    int br = access(generation, side, x + stepSize, y + stepSize);
    int b = access(generation, side, x + halfSize, y + stepSize + halfSize);
    int r = access(generation, side, x + stepSize + halfSize, y + halfSize);

    int lower = std::max({ bl,tl,tr,br,b,r });
    int upper = std::min({ bl,tl,tr,br,b,r });

    if (y > 0) {
        int t = access(generation, side, x + halfSize, y - stepSize);
        lower = std::max(t, lower);
        upper = std::min(t, upper);
    }
    if (x > 0) {
        int l = access(generation, side, x - halfSize, y + halfSize);
        lower = std::max(l, lower);
        upper = std::min(l, upper);
    }

    lower -= 2;
    upper += 2;

    int avg = GetRandomValue(lower, upper);
    set(generation, x + halfSize, y + halfSize, avg);
};

// i,j is center of a diamond
void square(std::vector<std::vector<int>>* generation, int side, int x, int y, int stepSize)
{
    int halfSize = stepSize / 2;
    // Wrap around makes them always available
    int b = access(generation, side, x, y + halfSize);
    int r = access(generation, side, x + halfSize, y);
    int lower = std::max({ b,r });
    int upper = std::min({ b,r });

    if (x > stepSize) {
        int l = access(generation, side, x - halfSize, y);
        int lower = std::max(l, lower);
        int upper = std::min(l, upper);
    }
    if (y > stepSize) {
        int t = access(generation, side, x, y - halfSize);
        int lower = std::max(t, lower);
        int upper = std::min(t, upper);
    }

    lower -= 2;
    upper += 2;
    int avg = GetRandomValue(lower, upper);
    set(generation, x, y, avg);
};

// Generates a noise image using Diamond Square algorithm
// Image is a square of size 2^n + 1
// 1->3, 2->5, 3->9, 4->17, 5->33
std::vector<std::vector<int>>* GenerateHeightMap(int n)
{
    if (n < 0) return {};
    int side = pow(2, n) + 1;
    std::vector<std::vector<int>>* generation = new std::vector<std::vector<int>>(side, std::vector<int>(side, 0));

    //int randmin = -1;
    //int randmax = 1;
    set(generation, 0, 0, GetRandomValue(randmin, randmax));
    set(generation, 0, side - 1, GetRandomValue(randmin, randmax));
    set(generation, side - 1, 0, GetRandomValue(randmin, randmax));
    set(generation, side - 1, side - 1, GetRandomValue(randmin, randmax));

    for (int stepSize = side - 1; stepSize > 1; stepSize /= 2) {
        int halfSize = stepSize / 2;
        // Diamond
        // i,j is top corner of a square
        for (int j = 0; j < side - 1; j += stepSize)
            for (int i = 0; i < side - 1; i += stepSize)
                diamond(generation, side - 1, i, j, stepSize);

        // Square
        // i,j is center of a diamond
        for (int j = 0; j < side; j += halfSize)
            for (int i = (j + halfSize) % stepSize; i < side; i += stepSize)
                square(generation, side - 1, i, j, stepSize);
    }

    return generation;
}

TileMap GenerateTileMap(int n)
{
    std::vector<std::vector<int>>* generation = GenerateHeightMap(n);

    std::string line;
    for (int j = 0; j < std::pow(2, n) + 1; j++) {
        for (int i = 0; i < std::pow(2, n) + 1; i++) {
            line += std::to_string((*generation)[i][j]);
            line += ", ";
        }
        TraceLog(LOG_INFO, line.c_str());
        line.clear();
    }

    TileMap output(mapwidth, std::vector<TileMap::value_type::value_type>(mapheight, { 0 }));

    for (int y = 0; y < mapheight - 1; y++) {
        for (int x = 0; x < mapwidth - 1; x++) {
            //uint8_t corners = 0;
            MapTile& tile = output[x][y];
            int n = (*generation)[x][y];
            int w = (*generation)[x][y + 1];
            int e = (*generation)[x + 1][y];
            int s = (*generation)[x + 1][y + 1];
            //std::array<int, 4> tileCorners;
            int min = std::min({ n,w,e,s });
            int max = std::max({ n,w,e,s });
            if ((n - min) != 0)
                tile.corners |= NORTH_CORNER;
            if ((s - min) != 0)
                tile.corners |= EAST_CORNER;
            if ((w - min) != 0)
                tile.corners |= WEST_CORNER;
            if ((e - min) != 0)
                tile.corners |= SOUTH_CORNER;
            if (max - min == 2)
                tile.corners |= STEEP_CORNER;
            tile.height = min;
        }
    }

    delete generation;

    return output;
}


TileMap LoadHeightmap(Point* dim)
{
    Image heightmap = LoadImage("assets/Jerash.png");
    if (dim) {
        dim->x = heightmap.width;
        dim->y = heightmap.height;
    }
    
    TileMap output(heightmap.width, std::vector<TileMap::value_type::value_type>(heightmap.height, { 0 }));

    for (int j = 0; j < heightmap.height - 1; j++) {
        for (int i = 0; i < heightmap.width - 1; i++) {
            MapTile& tile = output[i][j];

            int n = GetImageColor(heightmap, i, j).b / 16;
            int w = GetImageColor(heightmap, i, j + 1).b / 16;
            int s = GetImageColor(heightmap, i + 1, j).b / 16;
            int e = GetImageColor(heightmap, i + 1, j + 1).b / 16;

            int min = std::min({ n,w,e,s });
            int max = std::max({ n,w,e,s });
            if ((n - min) != 0)
                tile.corners |= NORTH_CORNER;
            if ((s - min) != 0)
                tile.corners |= EAST_CORNER;
            if ((w - min) != 0)
                tile.corners |= WEST_CORNER;
            if ((e - min) != 0)
                tile.corners |= SOUTH_CORNER; // ????
            if (max - min == 2)
                tile.corners |= STEEP_CORNER;
            tile.height = min;
        }
    }

    UnloadImage(heightmap);
    return output;
}
