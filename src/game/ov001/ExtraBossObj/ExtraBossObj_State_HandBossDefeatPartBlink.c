/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void ExtraBossObj_State_HandBossDefeatPartBlink(u8 *p0) {
    *(u32 *)((u8 *)p0 + 0x14) = (*(u32 *)((u8 *)p0 + 0x14) ^ 0x30000);
    *(u32 *)((u8 *)p0 + 0x1c8) = (*(u32 *)((u8 *)p0 + 0x1c8) - 0x1);
    s32 t0 = *(u32 *)((u8 *)p0 + 0x1c8);
    if (t0 <= 0) {
        *(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x84) + 0x114) = 0x2000;
    }
}
