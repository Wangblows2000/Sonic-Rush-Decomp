/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gExtraBossWork[];
s32 ExtraBoss_Unk_ov001_022fd744();
s32 ExtraBossObj_DrawAll();
s32 ExtraBoss_Unk_ov001_02304028();

void ExtraBoss_Unk_ov001_022feee4(void) {
    s32 t0 = (*(u32 *)*(u32 *)gExtraBossWork & 4);
    if (t0 != 0) {
        ExtraBoss_Unk_ov001_022fd744(t0);
    }
    ExtraBossObj_DrawAll();
    ExtraBoss_Unk_ov001_02304028();
}
