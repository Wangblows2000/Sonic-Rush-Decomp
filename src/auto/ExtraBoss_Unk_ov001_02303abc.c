/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov001_0231f548[];

void ExtraBoss_Unk_ov001_02303abc(s32 p0) {
    u32 t0 = *(u32 *)data_ov001_0231f548;
    u32 t1 = *(u32 *)((u8 *)t0 + 0x94);
    *(u32 *)((u8 *)t0 + 0x94) = (t1 & ~1);
    u32 t2 = *(u32 *)data_ov001_0231f548;
    u32 t3 = *(u32 *)((u8 *)t2 + 0x94);
    *(u32 *)((u8 *)t2 + 0x94) = (t3 | (p0 != 0));
}
