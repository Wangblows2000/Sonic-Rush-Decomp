/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gMarkerTimeListTask[];
s32 Hud_FramesToTimeDigits();

void CheckpointTimePopup_SetTimeDigits(s32 p0, s32 p1) {
    s32 t0 = *(u32 *)gMarkerTimeListTask;
    if (t0 == 0) {
        return;
    }
    Hud_FramesToTimeDigits(((*(u32 *)((u8 *)t0 + 0x10) + 0x134) + ((s32)p0 << 2)), p1, (*(u32 *)((u8 *)t0 + 0x10) + 0x134));
}
