/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_02080f48[];
s32 _s32_div_f();

void func_0201a9a0(s32 p0) {
    *(u32 *)data_02080f48 = (u32)((u8 *)p0 + -0x2);
    s32 t0 = _s32_div_f(0x1e, ((u8 *)p0 + -0x2), data_02080f48);
    *(u32 *)((u8 *)data_02080f48 + 0x4) = t0;
    *(u32 *)((u8 *)data_02080f48 + 0x8) = 0x1e;
}
