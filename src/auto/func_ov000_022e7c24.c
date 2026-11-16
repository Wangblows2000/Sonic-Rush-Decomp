/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_02369b78[];
extern u8 data_ov000_02368ba2[];
s32 func_ov000_022e9b70();
void func_ov000_022e7bf4();

void func_ov000_022e7c24(u8 *p0) {
    *(u32 *)((u8 *)*(u32 *)data_ov000_02369b78 + 0x20c) = (*(u32 *)((u8 *)*(u32 *)data_ov000_02369b78 + 0x20c) & ~0x20);
    *(u16 *)((u8 *)p0 + 0x2e) = 0;
    *(u16 *)((u8 *)p0 + 0x30) = ((s32)(-*(s16 *)((u8 *)data_ov000_02368ba2 + (*(u32 *)((u8 *)p0 + 0x3d0) * 0x30)) + ((u32)-*(s16 *)((u8 *)data_ov000_02368ba2 + (*(u32 *)((u8 *)p0 + 0x3d0) * 0x30)) >> 0x1f)) >> 1);
    s32 t0 = (*(u32 *)((u8 *)p0 + 0x58) | 0x80);
    *(u32 *)((u8 *)p0 + 0x58) = t0;
    func_ov000_022e9b70(p0, 0, 0, t0);
    *(u32 *)((u8 *)p0 + 0x440) = (u32)func_ov000_022e7bf4;
}
