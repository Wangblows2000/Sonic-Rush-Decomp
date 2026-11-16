/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 ExtraBoss_Unk_ov001_022f6450();
s32 ExtraBoss_Unk_ov001_022f3b50();

void ExtraBossObj_State_Unk_ov001_022f53f0(u8 *p0) {
    *(u32 *)((u8 *)p0 + 0x1c8) = (*(u32 *)((u8 *)p0 + 0x1c8) - 0x1);
    s32 t0 = *(u32 *)((u8 *)p0 + 0x1c8);
    if (t0 <= 0) {
        ExtraBoss_Unk_ov001_022f6450(p0, t0);
    }
    ExtraBoss_Unk_ov001_022f3b50(p0);
}
