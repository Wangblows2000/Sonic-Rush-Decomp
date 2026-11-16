/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_023280e8();
s32 func_020366f4();
s32 func_ov000_022f4ce4();
s32 func_02036754();
void func_ov000_022f0158();

void func_ov000_022f01f8(u8 *p0) {
    s32 t0 = func_ov000_023280e8(0, 0x1a000, 0x30000, 0);
    *(u32 *)((u8 *)p0 + 0x434) = t0;
    *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x434) + 0x5c) = (*(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x434) + 0x5c) | 4);
    s32 t1 = *(u32 *)((u8 *)p0 + 0x434);
    *(u32 *)((u8 *)t1 + 0xc) = 0x270f;
    func_020366f4(0, 0, 0xd2, t1);
    func_ov000_022f4ce4(p0, 0x21, 0x1000, 1);
    func_02036754(0, 0, 0xc7);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_022f0158;
}
