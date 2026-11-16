#include <nitro/types.h>

extern u16 gDisplayMain[];
extern u16 gDisplaySub[];

#pragma thumb on
void TimeAttackMenuBgScroll_Main(void) {
    gDisplayMain[6] += 2;
    gDisplaySub[6] += 2;
}
#pragma thumb reset
