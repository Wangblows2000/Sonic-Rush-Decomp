#include <nitro/types.h>

extern u16 gDisplayMain[];
extern u16 gDisplaySub[];

#pragma thumb on
void ResultsScreen_ResetBgOffsets(void) {
    gDisplayMain[0] = 0;
    gDisplayMain[1] = 0;
    gDisplayMain[2] = 0;
    gDisplayMain[3] = 0;
    gDisplayMain[4] = 0;
    gDisplayMain[5] = 0;
    gDisplayMain[6] = 0;
    gDisplayMain[7] = 0;
    gDisplaySub[0] = 0;
    gDisplaySub[1] = 0;
    gDisplaySub[2] = 0;
    gDisplaySub[3] = 0;
    gDisplaySub[4] = 0;
    gDisplaySub[5] = 0;
    gDisplaySub[6] = 0;
    gDisplaySub[7] = 0;
}
#pragma thumb reset
