/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov001_0231f654[];
s32 MTX_TransApply43();

void func_ov001_0230d0f8(void) {
    MTX_TransApply43((*(u32 *)data_ov001_0231f654 + 0x8), (*(u32 *)data_ov001_0231f654 + 0x8), 0, 0, *(u32 *)*(u32 *)data_ov001_0231f654);
}
