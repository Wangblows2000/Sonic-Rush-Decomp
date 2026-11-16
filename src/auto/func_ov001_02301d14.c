/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov001_02300c94();
s32 func_ov001_023024d0();

void func_ov001_02301d14(void) {
    s32 t0 = func_ov001_02300c94();
    s32 t1 = *(u32 *)((u8 *)t0 + 0x1dc);
    func_ov001_023024d0(t0, 6);
    *(u32 *)((u8 *)t0 + 0x14) = (*(u32 *)((u8 *)t0 + 0x14) & 0xfffdf8ff);
    *(u32 *)t1 = (*(u32 *)t1 & ~1);
    *(u32 *)((u8 *)t0 + 0x14) = (*(u32 *)((u8 *)t0 + 0x14) | 0x1000);
    *(u32 *)((u8 *)t0 + 0xa0) = 0;
}
