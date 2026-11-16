/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_0236aad8[];
extern u8 data_ov000_0236ac60[];
s32 func_0200225c();

void func_ov000_0233b250(s32 p0, u8 *p1, u8 *p2) {
    s32 t0 = func_0200225c((*(u32 *)((u8 *)*(u32 *)((u8 *)data_ov000_0236ac60 + (*(s8 *)((u8 *)((s32)p0 * 0x64 + (s32)data_ov000_0236aad8) + 0x3a) << 2)) + 0x10) - *(u32 *)((u8 *)((s32)p0 * 0x64 + (s32)data_ov000_0236aad8) + 0x4)), *(s16 *)((u8 *)((s32)p0 * 0x64 + (s32)data_ov000_0236aad8) + 0x1e), p2, *(u32 *)((u8 *)*(u32 *)((u8 *)data_ov000_0236ac60 + (*(s8 *)((u8 *)((s32)p0 * 0x64 + (s32)data_ov000_0236aad8) + 0x3a) << 2)) + 0x10));
    *(u32 *)p1 = ((s32)t0 >> 4);
    s32 t1 = func_0200225c((*(u32 *)((u8 *)*(u32 *)((u8 *)data_ov000_0236ac60 + (*(s8 *)((u8 *)((s32)p0 * 0x64 + (s32)data_ov000_0236aad8) + 0x3a) << 2)) + 0x14) - *(u32 *)((u8 *)((s32)p0 * 0x64 + (s32)data_ov000_0236aad8) + 0x8)), *(s16 *)((u8 *)((s32)p0 * 0x64 + (s32)data_ov000_0236aad8) + 0x20), *(u32 *)((u8 *)*(u32 *)((u8 *)data_ov000_0236ac60 + (*(s8 *)((u8 *)((s32)p0 * 0x64 + (s32)data_ov000_0236aad8) + 0x3a) << 2)) + 0x14));
    *(u32 *)p2 = ((s32)t1 >> 4);
}
