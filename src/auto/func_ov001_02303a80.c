/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov001_0231f548[];

void func_ov001_02303a80(s32 p0) {
    *(u32 *)((u8 *)*(u32 *)data_ov001_0231f548 + 0x94) = (*(u32 *)((u8 *)*(u32 *)data_ov001_0231f548 + 0x94) & ~2);
    s32 t0 = (*(u32 *)((u8 *)*(u32 *)data_ov001_0231f548 + 0x94) | ((p0 != 0) ? 2 : 0));
    *(u32 *)((u8 *)*(u32 *)data_ov001_0231f548 + 0x94) = t0;
}
