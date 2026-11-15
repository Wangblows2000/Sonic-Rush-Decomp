/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022b4574[];
void func_ov001_022e6fbc();

void func_ov001_022e5d7c(s32 p0, s32 p1, s32 p2) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    *(u32 *)((u8 *)t0 + 0x50) = (*(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) + 0x6c) << 0xc);
    *(u32 *)((u8 *)t0 + 0x54) = (*(u32 *)((u8 *)t0 + 0x70) << 0xc);
    *(u32 *)((u8 *)t0 + 0x58) = p2;
    *(u32 *)((u8 *)t0 + 0x5c) = p0;
    *(u32 *)((u8 *)t0 + 0x60) = p1;
    *(u32 *)((u8 *)t0 + 0x58) = p2;
    *(u32 *)((u8 *)t0 + 0x74) = 0;
    s32 t1 = *(u32 *)data_022b4574;
    *(u32 *)((u8 *)t1 + 0x8) = (u32)func_ov001_022e6fbc;
}

#pragma thumb reset
