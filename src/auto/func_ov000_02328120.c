/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_02367c98[];
s32 func_ov000_02327ca4();

void func_ov000_02328120(s32 p0, s32 p1, s32 p2, s32 p3) {
    s32 t0 = func_ov000_02327ca4(data_ov000_02367c98, 7, p0, p1, p2, p3);
    *(u8 *)((u8 *)t0 + 0x13e) = 0x13;
    *(u8 *)((u8 *)t0 + 0x13f) = 3;
    *(u8 *)((u8 *)t0 + 0x140) = 0;
}
