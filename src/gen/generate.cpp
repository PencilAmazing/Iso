#include "generate.h"

// keep this above zero
int randmin = 2;
int randmax = 2;

short access(std::vector<std::vector<short>>& array, int side, int x, int y)
{
    //could be replaced with clearing the sign bit but eh
    //int i = abs(x) % (side + 1);
    //int j = abs(y) % (side + 1);
    // this assumes side is a power of two
    int i = x & side;
    int j = y & side;
    return array[i][j];
};

void set(std::vector<std::vector<short>>& array, int x, int y, int value)
{
    array[x][y] = value;
};

// i,j is top left corner of a square
void diamond(std::vector<std::vector<short>>& generation, int side, int x, int y, int stepSize)
{
    int halfSize = stepSize / 2;

    // variable names are relative to center of the diamond
    short bl = access(generation, side, x, y + stepSize);
    short tl = access(generation, side, x, y);
    short tr = access(generation, side, x + stepSize, y);
    short br = access(generation, side, x + stepSize, y + stepSize);

    short t = access(generation, side, x + halfSize, y - stepSize);
    short r = access(generation, side, x + stepSize + halfSize, y + halfSize);
    short b = access(generation, side, x + halfSize, y + stepSize + halfSize);
    short l = access(generation, side, x - halfSize, y + halfSize);

    std::array<short, 8> n = { bl,tl,tr,br,t,r,b,l };

    short lower = *std::max_element(n.begin(), n.end()) - halfSize;
    short upper = *std::min_element(n.begin(), n.end()) + halfSize;

    short avg = GetRandomValue(lower, upper);
    set(generation, x + halfSize, y + halfSize, avg);
};

// i,j is center of a diamond
void square(std::vector<std::vector<short>>& generation, int side, int x, int y, int stepSize)
{
    int halfSize = stepSize / 2;
    //short lower, upper;
    short t = access(generation, side, x, y - halfSize);
    short r = access(generation, side, x + halfSize, y);
    short b = access(generation, side, x, y + halfSize);
    short l = access(generation, side, x - halfSize, y);
    // Height can change by one per step
    short lower = std::max({ t,r,b,l }) - halfSize;
    short upper = std::min({ t,r,b,l }) + halfSize;
    short avg = GetRandomValue(lower, upper);
    set(generation, x, y, avg);
};

// Generates a noise image using Diamond Square algorithm
// Image is a square of size 2^n + 1
// 1->3, 2->5, 3->9, 4->17, 5->33
std::vector<std::vector<short>> GenerateHeightMap(int n)
{
    if (n < 0) return {};
    int side = pow(2, n) + 1;
    std::vector<std::vector<short>> generation = std::vector<std::vector<short>>(side, std::vector<short>(side, 0));

    //int randmin = -1;
    //int randmax = 1;
    generation[0][0] = GetRandomValue(randmin, randmax);
    generation[0][side - 1] = GetRandomValue(randmin, randmax);
    generation[side - 1][0] = GetRandomValue(randmin, randmax);
    generation[side - 1][side - 1] = GetRandomValue(randmin, randmax);

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
            for (int i = (j + halfSize) % stepSize; i < side; i += stepSize) {
                square(generation, side - 1, i, j, stepSize);
            }
    }

    return generation;
}

TileMap GenerateTileMap(int n)
{
    std::vector<std::vector<short>> generation = GenerateHeightMap(n);

    for (int i = 0; i < std::pow(2, n) + 1; i++) {
        std::string line;
        for (int j = 0; j < std::pow(2, n) + 1; j++) {
            line += std::to_string(generation[i][j]);
            line += ", ";
        }
        TraceLog(LOG_INFO, line.c_str());
    }

    TileMap output(mapwidth, std::vector<TileMap::value_type::value_type>(mapheight, { 0 }));

    for (int y = 0; y < mapheight; y++) {
        for (int x = 0; x < mapwidth; x++) {
            break;
            //uint8_t corners = 0;
            MapTile& tile = output[x][y];
            short n = generation[x][y];
            short w = generation[x][y+1];
            short e = generation[x + 1][y];
            short s = generation[x + 1][y + 1];
            std::array<short, 4> tileCorners;
            short min = std::min({ n,w,e,s });
            //short max = std::max({ n,w,e,s });

            if ((n - min) != 0) tile.corners |= NORTH_CORNER;
            if ((s - min) != 0) tile.corners |= SOUTH_CORNER;
            if ((e - min) != 0) tile.corners |= EAST_CORNER;
            if ((w - min) != 0) tile.corners |= WEST_CORNER;
            tile.height = min;
        }
    }

    //for (int i = 0; i < mapwidth + 1; i++)
    //    delete[] generation[i];
    //delete[] generation;

    return output;
}
