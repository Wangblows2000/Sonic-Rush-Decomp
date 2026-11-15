/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov001_0231f654[];
s32 func_02001c00();

void func_ov001_0230d0f8(void) {
    s32 t0 = func_02001c00((*(u32 *)data_ov001_0231f654 + 0x8), (*(u32 *)data_ov001_0231f654 + 0x8), 0, 0, *(u32 *)*(u32 *)data_ov001_0231f654);
}
