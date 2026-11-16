/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022503e0[];
s32 func_020351d8();
s32 func_02035134();

void func_020352a8(void) {
    func_020351d8(data_022503e0, 0, 0);
    u16 t0 = *(volatile u16 *)0x4000130;
    u16 t1 = *(u16 *)0x27fffa8;
    func_02035134(data_022503e0, ((u32)((((t0 | t1) ^ 0x2fff) & 0x2fff) << 0x10) >> 0x10), ((t0 | t1) ^ 0x2fff), t0);
}
