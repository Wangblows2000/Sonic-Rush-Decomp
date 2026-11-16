/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gExtraBossResultTime[];
extern u8 gExtraBossResultRings[];

void ExtraBoss_SetTimeAndRings(s32 p0, s32 p1) {
    *(u32 *)gExtraBossResultTime = p0;
    if ((s32)p0 > 0x8c9f) {
        *(u32 *)gExtraBossResultTime = 0x8c9f;
    } else {
        if ((s32)p0 < 0) {
            *(u32 *)gExtraBossResultTime = 0;
        }
    }
    *(u32 *)gExtraBossResultRings = p1;
    if ((s32)p1 > 0x3e7) {
        *(u32 *)gExtraBossResultRings = 0x3e7;
        return;
    }
    if ((s32)p1 < 0) {
        *(u32 *)gExtraBossResultRings = 0;
    }
}
