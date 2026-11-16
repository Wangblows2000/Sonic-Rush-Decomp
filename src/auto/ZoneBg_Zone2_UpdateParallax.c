/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Camera_GetOtherBySwapState();
s32 FX_Div();

void ZoneBg_Zone2_UpdateParallax(u8 *p0) {
    s32 t0 = Camera_GetOtherBySwapState();
    if ((*(u32 *)((u8 *)t0 + 0x34) & 1) != 0) {
        return;
    }
    *(u32 *)((u8 *)t0 + 0x2c) = (0xffff & ((s32)*(u32 *)((u8 *)t0 + 0x4) >> 4));
    *(u32 *)((u8 *)t0 + 0x30) = 0x14000;
    *(u32 *)((u8 *)p0 + 0x2c) = (0xffff & ((s32)*(u32 *)((u8 *)t0 + 0x4) >> 4));
    *(u32 *)((u8 *)p0 + 0x30) = 0x14000;
    if ((*(u32 *)t0 & 0x1000000) != 0) {
        s32 t1 = FX_Div(((*(u16 *)((u8 *)t0 + 0x62) << 8) - *(u32 *)((u8 *)t0 + 0x8)), *(s16 *)((u8 *)t0 + 0x20), *(u32 *)((u8 *)t0 + 0x8), *(u16 *)((u8 *)t0 + 0x62));
        s32 v2 = ((s32)(((s32)t1 >> 0xc) << 0x10) >> 0x10);
        if (((s32)(((s32)t1 >> 0xc) << 0x10) >> 0x10) < 0) {
            v2 = 0;
        } else {
            s32 v3 = v2;
            if (v2 > 0xc0) {
                v3 = 0xc0;
            }
            v2 = v3;
        }
        *(u8 *)((u8 *)p0 + 0x41) = v2;
        return;
    }
    *(u8 *)((u8 *)p0 + 0x41) = 0xc0;
}
