/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_02363f4c[];
void func_ov000_022f32d0();

void func_ov000_022f338c(u8 *p0) {
    s32 t0 = *(u16 *)((u8 *)data_ov000_02363f4c + (*(u32 *)((u8 *)p0 + 0x458) << 1));
    *(u16 *)((u8 *)p0 + 0x434) = t0;
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_022f32d0;
    ((s32 (*)())*(u32 *)((u8 *)p0 + 0x68))(p0, *(u32 *)((u8 *)p0 + 0x68), (u32)func_ov000_022f32d0, t0);
}
