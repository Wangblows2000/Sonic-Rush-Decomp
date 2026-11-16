/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void func_ov001_022f7700();
s32 func_020372d0();
extern u8 data_ov001_0231f084[];

void func_ov001_022f7740(void) {
    s32 t0 = func_020372d0((u32)func_ov001_022f7700, 0, 0, 0x1e00, 0, 0);
    *(u32 *)((u8 *)*(u32 *)data_ov001_0231f084 + 0xe20) = t0;
    *(u32 *)((u8 *)*(u32 *)data_ov001_0231f084 + 0xe9c) = 0x3c;
}
