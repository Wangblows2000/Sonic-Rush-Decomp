/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void func_ov001_02305bb8();
s32 func_020372d0();
extern u8 data_ov001_0231f5a8[];

void func_ov001_02305c84(void) {
    s32 t0 = func_020372d0((u32)func_ov001_02305bb8, 0, 0, 0x3100, 2, 0);
    *(u32 *)((u8 *)*(u32 *)data_ov001_0231f5a8 + 0xc) = t0;
    *(u32 *)((u8 *)*(u32 *)data_ov001_0231f5a8 + 0x5ec) = 0;
}
