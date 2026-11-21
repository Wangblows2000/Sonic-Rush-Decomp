/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gCamera[];
s32 StageEvent_SpawnInViewSingleScreen();

void StageEvent_UpdateSingleScreen(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)gCamera + 0x4);
    if (t0 == *(u32 *)((u8 *)p0 + 0xc)) {
        if (*(u32 *)((u8 *)gCamera + 0x8) == *(u32 *)((u8 *)p0 + 0x10)) {
            return;
        }
    }
    StageEvent_SpawnInViewSingleScreen(p0, 0, 5, t0);
}
