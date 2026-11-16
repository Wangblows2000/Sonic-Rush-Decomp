/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 ZoneMap_Unk_ov001_022e21b8();
s32 ZoneMap_Unk_ov001_022e5a48();

void ZoneMap_Unk_ov001_022e5de0(s32 p0, s32 p1) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    s32 t1 = (*(u32 *)(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0xa4) + 0x674);
    s32 t2 = ((*(u32 *)(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0xa4) + 0x674) + ((s32)p0 << 4));
    s32 v3 = (s32)p1;
    if ((s32)p1 == 0x12) {
        v3 = 0;
    } else {
        v3 = (t1 + (v3 << 4));
    }
    *(u32 *)((u8 *)t2 + 0xc) = 0;
    if (v3 != 0) {
        *(u32 *)((u8 *)v3 + 0xc) = 5;
    }
    ZoneMap_Unk_ov001_022e21b8(t0, v3, t0, t1);
    ZoneMap_Unk_ov001_022e5a48(0xa);
}

#pragma thumb reset
