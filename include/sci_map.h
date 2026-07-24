#include <utility>
#include <stdexcept>

constexpr int channel_map[16][16] = {
    {109,111,113,115,117,119,121,123,268,266,264,262,258,256,254,252}, // y=0
    {126,128,130,132,136,138,140,142,285,283,281,279,277,275,273,271}, // y=1
    { 73, 75, 77, 79, 83, 85, 87, 88,235,237,239,241,243,245,247,249}, // y=2
    { 90, 92, 94, 96,100,102,104,106,216,218,220,222,225,226,228,230}, // y=3
    { 72, 74, 76, 78, 81, 82, 84, 86,234,236,238,240,244,246,248,250}, // y=4
    { 91, 93, 95, 97, 99,101,103,105,217,219,221,223,227,229,231,232}, // y=5
    {141,139,137,135,133,131,129,127,270,272,274,276,280,282,284,286}, // y=6
    {124,122,120,118,114,112,110,108,253,255,257,259,261,263,265,267}, // y=7
    { 37, 39, 41, 43, 45, 47, 49, 51,196,194,192,190,186,184,182,180}, // y=8
    { 54, 56, 58, 60, 64, 66, 68, 70,213,211,209,207,205,203,201,199}, // y=9
    {  1,  3,  5,  7, 11, 13, 15, 16,163,165,167,169,171,173,175,177}, // y=10
    { 18, 20, 22, 24, 28, 30, 32, 34,144,146,148,150,153,154,156,158}, // y=11
    {  0,  9, 19, 27, 69, 61, 52, 42,162,164,166,168,172,174,176,178}, // y=12
    {  2, 10, 21, 29, 67, 59, 50, 40,145,147,149,151,155,157,159,160}, // y=13
    {  4, 12, 23, 31, 65, 57, 48, 38,198,200,202,204,208,210,212,214}, // y=14
    {  6, 14, 25, 33, 63, 55, 46, 36,181,183,185,187,189,191,193,195}  // y=15
};

constexpr int sci_map[4][4] = {
    {13, 14, 15, 16},
    {9, 10, 11, 12},
    {5, 6, 7, 8},
    {1, 2, 3, 4}
};

int ch_to_sci(int ch)
{
    int x = -1;
    int y = -1;

    for(int iy=0; iy<16; iy++){
        for(int ix=0; ix<16; ix++){
            if(channel_map[iy][ix] == ch){
                x = ix;
                y = iy;
            }
        }
    }

    if(x < 0) throw std::runtime_error("channel not found");

    int sci_x = x / 4;
    int sci_y = y / 4;

    return sci_map[sci_y][sci_x];
}

std::pair<int,int> ch_to_xy(int ch)
{
    for(int y=0;y<16;y++){
        for(int x=0;x<16;x++){
            if(channel_map[y][x] == ch)
                return {x,y};
        }
    }

    throw std::runtime_error("channel not found");
}

std::pair<int,int> sci_to_xy(int sci)
{
    for(int y=0;y<4;y++){
        for(int x=0;x<4;x++){
            if(sci_map[y][x]==sci)
                return {x,y};
        }
    }

    throw std::runtime_error("sci not found");
}

int xy_to_ch(std::pair<int,int> xy)
{
    int x = xy.first;
    int y = xy.second;

    if(x < 0 || x >= 16 || y < 0 || y >= 16) throw std::runtime_error("invalid ch coordinate");

    return channel_map[y][x];
}

int xy_to_sci(std::pair<int,int> xy)
{
    int x = xy.first;
    int y = xy.second;

    if(x < 0 || x >= 4 || y < 0 || y >= 4) throw std::runtime_error("invalid sci coordinate");

    return sci_map[y][x];
}

bool is_near_sci(int target_sci, int leading_sci)
{
    int target_x = -1;
    int target_y = -1;

    int leading_x = -1;
    int leading_y = -1;

    for(int y=0; y<4; y++){
        for(int x=0; x<4; x++){

            if(sci_map[y][x] == target_sci){
                target_x = x;
                target_y = y;
            }

            if(sci_map[y][x] == leading_sci){
                leading_x = x;
                leading_y = y;
            }
        }
    }

    if(target_x < 0 || leading_x < 0) return false;

    if(std::abs(target_x - leading_x) <= 1 && std::abs(target_y - leading_y) <= 1){
        return true;
    }

    return false;
}