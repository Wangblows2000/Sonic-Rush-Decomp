/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 ExtraBoss_Unk_ov001_022f3b50();
s32 ExtraBoss_Unk_ov001_022f6450();

void ExtraBossObj_State_Unk_ov001_022f61ec(u8 *p0) {
    *(u32 *)((u8 *)p0 + 0x1c8) = (*(u32 *)((u8 *)p0 + 0x1c8) - 0x1);
    s32 t0 = *(u32 *)((u8 *)p0 + 0x1c8);
    if (t0 > 0) {
        ExtraBoss_Unk_ov001_022f3b50(p0, t0);
        return;
    }
    ExtraBoss_Unk_ov001_022f6450();
}
