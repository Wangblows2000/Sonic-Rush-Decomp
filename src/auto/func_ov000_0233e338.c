/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_0233d82c();
s32 func_ov000_0233cc34();
void func_ov000_0233c794();
s32 func_ov000_0233d3b4();
void func_ov000_0233d65c();
void func_ov000_0233d7e8();
s32 func_020372d0();

void func_ov000_0233e338(u8 *p0) {
    func_ov000_0233d82c(p0);
    func_ov000_0233cc34(p0);
    *(u32 *)((u8 *)p0 + 0x8b4) = (u32)func_ov000_0233c794;
    func_ov000_0233d3b4((u32)func_ov000_0233c794);
    func_020372d0((u32)func_ov000_0233d65c, (u32)func_ov000_0233d7e8, 7, 0x3f10, 3, 0);
}
