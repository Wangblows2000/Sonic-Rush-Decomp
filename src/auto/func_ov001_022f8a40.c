/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov001_0231f084[];
void func_ov001_022f8858();
s32 func_020372d0();

void func_ov001_022f8a40(void) {
    *(u32 *)((u8 *)*(u32 *)data_ov001_0231f084 + 0xe4c) = 0;
    *(u32 *)((u8 *)*(u32 *)data_ov001_0231f084 + 0xe50) = 0;
    s32 t0 = func_020372d0((u32)func_ov001_022f8858, 0, 0, 0x2000, 0, 0);
    *(u32 *)((u8 *)*(u32 *)data_ov001_0231f084 + 0xe14) = t0;
}
