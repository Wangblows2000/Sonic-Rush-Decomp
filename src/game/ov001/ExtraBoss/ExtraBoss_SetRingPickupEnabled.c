/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gExtraBossRingWork[];

void ExtraBoss_SetRingPickupEnabled(s32 p0) {
    *(u32 *)((u8 *)*(u32 *)gExtraBossRingWork + 0x94) = (*(u32 *)((u8 *)*(u32 *)gExtraBossRingWork + 0x94) & ~2);
    s32 t0 = (*(u32 *)((u8 *)*(u32 *)gExtraBossRingWork + 0x94) | ((p0 != 0) ? 2 : 0));
    *(u32 *)((u8 *)*(u32 *)gExtraBossRingWork + 0x94) = t0;
}
