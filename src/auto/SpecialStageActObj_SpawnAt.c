/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov001_02306db0();

s32 SpecialStageActObj_SpawnAt(s32 p0, s32 p1, s32 p2, s32 p3) {
    s32 t0 = func_ov001_02306db0(0, p0);
    if (t0 == 0) {
        return 0;
    }
    *(u32 *)((u8 *)t0 + 0x20) = p1;
    *(u32 *)((u8 *)t0 + 0x24) = p2;
    *(u32 *)((u8 *)t0 + 0x28) = p3;
    *(u16 *)((u8 *)t0 + 0x60) = (*(u16 *)((u8 *)t0 + 0x60) | 0x20);
    return t0;
}
