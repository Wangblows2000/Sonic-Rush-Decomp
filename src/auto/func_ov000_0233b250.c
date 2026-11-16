/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gCamera[];
extern u8 gPlayers[];
s32 FX_Div();

void func_ov000_0233b250(s32 p0, u8 *p1, u8 *p2) {
    s32 t0 = FX_Div((*(u32 *)((u8 *)*(u32 *)((u8 *)gPlayers + (*(s8 *)((u8 *)((s32)p0 * 0x64 + (s32)gCamera) + 0x3a) << 2)) + 0x10) - *(u32 *)((u8 *)((s32)p0 * 0x64 + (s32)gCamera) + 0x4)), *(s16 *)((u8 *)((s32)p0 * 0x64 + (s32)gCamera) + 0x1e), p2, *(u32 *)((u8 *)*(u32 *)((u8 *)gPlayers + (*(s8 *)((u8 *)((s32)p0 * 0x64 + (s32)gCamera) + 0x3a) << 2)) + 0x10));
    *(u32 *)p1 = ((s32)t0 >> 4);
    s32 t1 = FX_Div((*(u32 *)((u8 *)*(u32 *)((u8 *)gPlayers + (*(s8 *)((u8 *)((s32)p0 * 0x64 + (s32)gCamera) + 0x3a) << 2)) + 0x14) - *(u32 *)((u8 *)((s32)p0 * 0x64 + (s32)gCamera) + 0x8)), *(s16 *)((u8 *)((s32)p0 * 0x64 + (s32)gCamera) + 0x20), *(u32 *)((u8 *)*(u32 *)((u8 *)gPlayers + (*(s8 *)((u8 *)((s32)p0 * 0x64 + (s32)gCamera) + 0x3a) << 2)) + 0x14));
    *(u32 *)p2 = ((s32)t1 >> 4);
}
