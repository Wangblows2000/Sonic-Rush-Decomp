/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 ExtraBoss_Unk_ov001_022f2ca8();

void ExtraBossObj_State_Unk_ov001_022f0b40(u8 *p0) {
    s32 t0 = (*(u8 *)((u8 *)*(u32 *)((u8 *)p0 + 0x84) + 0x10c) & 2);
    if (t0 == 0) {
        return;
    }
    ExtraBoss_Unk_ov001_022f2ca8(p0, t0);
}
