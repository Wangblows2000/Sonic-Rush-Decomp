/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_ov001_02316294[];

void ZoneMap_Unk_ov001_022e3844(u8 *p0, s32 p1) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x1c);
    if (t0 != (s32)p1) {
        *(u32 *)((u8 *)p0 + 0x20) = t0;
        *(u32 *)((u8 *)p0 + 0x1c) = p1;
        if (*(u32 *)((u8 *)data_ov001_02316294 + ((s32)p1 << 2)) != 0) {
            *(u8 *)((u8 *)p0 + 0x130) = (*(u8 *)((u8 *)p0 + 0x130) | 4);
        } else {
            *(u8 *)((u8 *)p0 + 0x130) = (*(u8 *)((u8 *)p0 + 0x130) & ~4);
        }
        if (*(u32 *)p0 == 0) {
            *(u32 *)((u8 *)p0 + 0x138) = 0x1000;
            return;
        }
        if ((s32)p1 == 1) {
            *(u32 *)((u8 *)p0 + 0x138) = 0x2000;
            return;
        }
        *(u32 *)((u8 *)p0 + 0x138) = 0x1000;
    }
}

#pragma thumb reset
