/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void func_ov000_022cf2d0();
s32 func_020446b0();
s32 func_02036754();

void func_ov000_022cf38c(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x210) + 0x10);
    *(u32 *)((u8 *)t0 + 0x4) = (u32)func_ov000_022cf2d0;
    func_020446b0(0xc, (u32)func_ov000_022cf2d0);
    *(u32 *)((u8 *)t0 + 0xc) = 0xf;
    func_02036754(*(u32 *)((u8 *)p0 + 0x1454), 0, 0x9f);
}
