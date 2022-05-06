#https://newgrf-specs.tt-wiki.net/wiki/NML:List_of_tile_slopes
# the authority on isometric geography
# or is it?
# anyways, this should be reviewed everytime spritesheet is changed
# because the orientations are hardcoded and I can't be bothered to
# find a clever solution

NORTH_CORNER = 1 << 0
EAST_CORNER = 1 << 1
SOUTH_CORNER = 1 << 2
WEST_CORNER = 1 << 3
STEEP_CORNER = 1 << 4

# return direction and slope for each pattern
lookup_table = []

opposite_direction = {'n':'Tile_South', 'e':'Tile_West', 's':'Tile_North', 'w': 'Tile_East'}

def test_bit(i, bit):
    return (i & bit) == bit

def read_pattern(pattern):
    out = ["Tile_Invalid", "Tile_North"]
    # Three corners raised slope
    if len(pattern.replace('q','')) == 3:
        if 'q' in pattern:
            out[0] = "Tile_Steep"
        else:
            out[0] = "Tile_ThreeQuarter"
        for dir in opposite_direction:
            if dir not in pattern:
                out[1] = opposite_direction[dir]
    elif len(pattern) == 2: # Two corners raised
        # saddle
        if 'n' in pattern and 's' in pattern:
            out[0] = "Tile_Saddle"
            out[1] = "Tile_North"
        elif 'e' in pattern and 'w' in pattern:
            out[0] = "Tile_Saddle"
            out[1] = "Tile_East"
        else:
            out[0] = "Tile_Slope"
            # Normal slope
            if 'n' in pattern:
                if 'w' in pattern:
                    out[1] = "Tile_West"
                elif 'e' in pattern:
                    out[1] = "Tile_North"
            elif 's' in pattern:
                if 'w' in pattern:
                    out[1] = "Tile_South"
                elif 'e' in pattern:
                    out[1] = "Tile_East"
    elif len(pattern) == 1:
        out[0] = "Tile_Corner"
        if pattern == 'n':
            out[1] = "Tile_North"
        elif pattern == 'e':
            out[1] = "Tile_East"
        elif pattern == 's':
            out[1] = "Tile_South"
        elif pattern == 'w':
            out[1] = "Tile_West"
    elif len(pattern) == 0:
        out[0] = "Tile_Flat"
        out[1] = "Tile_North"
    return out
            
print("const TileDescription tile_lookup_table[] = {")
for i in range(32):
    directions = ""
    if test_bit(i, NORTH_CORNER):
        directions += 'n'
    if test_bit(i, EAST_CORNER):
        directions += 'e'
    if test_bit(i, SOUTH_CORNER):
        directions += 's'
    if test_bit(i, WEST_CORNER):
        directions += 'w'
    if test_bit(i, STEEP_CORNER):
        directions += 'q'
    tile = read_pattern(directions)
    #print("\t{:08b} -> {}, {}".format(i, tile[0], tile[1]))
    print("\t{"+tile[0]+', '+tile[1]+' },')
print("};")
#print(lookup_table)

