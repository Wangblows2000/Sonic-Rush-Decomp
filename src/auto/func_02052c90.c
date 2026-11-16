/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_0207b8d4[];
extern u8 data_022c466c[];
s32 func_020525d8();

void func_02052c90(u8 *p0, s32 p1) {
    *(u32 *)((u8 *)p0 + 0xe4) = p1;
    *(u32 *)((u8 *)p0 + 0xec) = 0;
    s32 t0 = *(u32 *)((u8 *)data_0207b8d4 + (*(u32 *)((u8 *)p0 + 0x4) << 2));
    *(u32 *)((u8 *)p0 + 0xf0) = *(u32 *)((u8 *)*(u32 *)((u8 *)data_0207b8d4 + (*(u32 *)((u8 *)p0 + 0x4) << 2)) + ((s32)p1 << 2));
    *(u32 *)((u8 *)p0 + 0xe8) = 0;
    *(u32 *)((u8 *)p0 + 0xf4) = *(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0xf0) + 0x4);
    *(u32 *)((u8 *)p0 + 0xd4) = (*(u32 *)((u8 *)p0 + 0xd4) & ~0x100);
    s32 t1 = *(u32 *)data_022c466c;
    *(u32 *)((u8 *)p0 + 0xc) = (*(u8 *)((u8 *)*(u32 *)((u8 *)p0 + 0xf4) + 0x1) * 0x44 + *(u32 *)data_022c466c);
    func_020525d8(*(u32 *)((u8 *)p0 + 0xc), *(u32 *)((u8 *)p0 + 0x8), t1, t0);
    *(u32 *)((u8 *)p0 + 0xd4) = (*(u32 *)((u8 *)p0 + 0xd4) | 0x40000);
}
