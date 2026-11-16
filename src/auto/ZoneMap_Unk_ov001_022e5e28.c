/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
void ZoneMap_Unk_ov001_022e7dd8();

void ZoneMap_Unk_ov001_022e5e28(s32 p0, s32 p1, s32 p2) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    *(u32 *)((u8 *)t0 + 0x44) = p0;
    *(u32 *)((u8 *)t0 + 0x48) = p2;
    *(u32 *)((u8 *)t0 + 0x4c) = p1;
    *(u32 *)((u8 *)t0 + 0x50) = (*(u32 *)((u8 *)t0 + 0x6c) << 0xc);
    *(u32 *)((u8 *)t0 + 0x54) = (*(u32 *)((u8 *)t0 + 0x70) << 0xc);
    *(u32 *)((u8 *)t0 + 0x58) = 0x3000;
    s32 t1 = *(u32 *)gTaskCurrent;
    *(u32 *)((u8 *)t1 + 0x8) = (u32)ZoneMap_Unk_ov001_022e7dd8;
}

#pragma thumb reset
