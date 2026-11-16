/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_02007318();
void func_01fff7c4();
s32 func_020073b8();
void func_01fff760();

void func_ov000_0233d43c(s32 p0, s32 p1) {
    func_02007318(((s32)((u8 *)p0 + 0xc) + ((s32)p1 * 0x28)));
    func_02007318(((s32)((u8 *)p0 + 0x5c) + ((s32)p1 * 0x28)));
    func_020073b8(((s32)((u8 *)p0 + 0x5c) + ((s32)p1 * 0x28)), *(u8 *)((u8 *)((s32)p0 + (s32)p1) + 0xa), 5, (u32)func_01fff7c4, p1);
    func_020073b8(((s32)((u8 *)p0 + 0xc) + ((s32)p1 * 0x28)), 0xc8, 5, (u32)func_01fff760, p1);
}
