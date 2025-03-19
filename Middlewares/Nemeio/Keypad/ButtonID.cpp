#include "ButtonID.hpp"

etl::map<uint8_t, uint8_t, ButtonID::INDEX_COUNT> ButtonID::ScanCodeToKeyIndex =
    {etl::pair{0, 0},    etl::pair{7, 1},    etl::pair{14, 2},  etl::pair{21, 3},
     etl::pair{28, 4},   etl::pair{35, 5},   etl::pair{42, 6},  etl::pair{49, 7},
     etl::pair{56, 8},   etl::pair{63, 9},   etl::pair{70, 10}, etl::pair{77, 11},
     etl::pair{84, 12},  etl::pair{91, 13},  etl::pair{98, 14},

     etl::pair{1, 15},   etl::pair{8, 16},   etl::pair{15, 17}, etl::pair{22, 18},
     etl::pair{29, 19},  etl::pair{36, 20},  etl::pair{43, 21}, etl::pair{50, 22},
     etl::pair{57, 23},  etl::pair{64, 24},  etl::pair{71, 25}, etl::pair{78, 26},
     etl::pair{85, 27},  etl::pair{92, 28},

     etl::pair{2, 29},   etl::pair{9, 30},   etl::pair{16, 31}, etl::pair{23, 32},
     etl::pair{30, 33},  etl::pair{37, 34},  etl::pair{44, 35}, etl::pair{51, 36},
     etl::pair{58, 37},  etl::pair{65, 38},  etl::pair{72, 39}, etl::pair{79, 40},
     etl::pair{86, 41},  etl::pair{93, 42},

     etl::pair{3, 43},   etl::pair{10, 44},  etl::pair{17, 45}, etl::pair{24, 46},
     etl::pair{31, 47},  etl::pair{38, 48},  etl::pair{45, 49}, etl::pair{52, 50},
     etl::pair{59, 51},  etl::pair{66, 52},  etl::pair{73, 53}, etl::pair{80, 54},
     etl::pair{27, 55},

     etl::pair{34, 56},  etl::pair{11, 57},  etl::pair{18, 58}, etl::pair{25, 59},
     etl::pair{32, 60},  etl::pair{39, 61},  etl::pair{46, 62}, etl::pair{53, 63},
     etl::pair{60, 64},  etl::pair{67, 65},  etl::pair{74, 66}, etl::pair{81, 67},
     etl::pair{102, 68}, etl::pair{41, 69},

     etl::pair{48, 70},  etl::pair{13, 71},  etl::pair{20, 72}, etl::pair{6, 73},
     etl::pair{33, 74},  etl::pair{62, 75},  etl::pair{55, 76}, etl::pair{82, 77},
     etl::pair{103, 78}, etl::pair{110, 79},

     etl::pair{69, 80},  etl::pair{76, 81}};

bool ButtonID::operator==(const ButtonID& button) const
{
    return (button.x == x) && (button.y == y);
}

bool ButtonID::toIndex(uint8_t& index) const
{
    bool ret = false;
    uint8_t scanCode = KeypadDefs::MAX_ROW * x + y;

    if(auto it = ScanCodeToKeyIndex.find(scanCode); it != ScanCodeToKeyIndex.end())
    {
        index = it->second;
        ret = true;
    }
    return ret;
}