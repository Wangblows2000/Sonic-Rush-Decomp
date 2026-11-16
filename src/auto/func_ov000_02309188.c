/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_022fad3c();
void func_ov000_023090e8();

void func_ov000_02309188(u8 *p0) {
    s32 t0 = func_ov000_022fad3c(*(u8 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1e0) + 0x7), *(u32 *)((u8 *)p0 + 0x10), *(u32 *)((u8 *)p0 + 0x14), ((u32)((*(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1e0) + 0x4) | 0x40) << 0x10) >> 0x10), *(s8 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1e0) + 0x6), 0, *(u8 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1e0) + 0x8), 0, 0);
    *(u32 *)((u8 *)p0 + 0x8) = t0;
    *(u32 *)((u8 *)t0 + 0x54) = (*(u32 *)((u8 *)t0 + 0x54) | 0x10);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_023090e8;
}
