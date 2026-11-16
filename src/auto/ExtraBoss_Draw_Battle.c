/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gExtraBossWork[];
s32 ExtraBoss_DrawHud();
s32 ExtraBossObj_DrawAll();
s32 ExtraBoss_DrawRings();

void ExtraBoss_Draw_Battle(void) {
    s32 t0 = (*(u32 *)*(u32 *)gExtraBossWork & 4);
    if (t0 != 0) {
        ExtraBoss_DrawHud(t0);
    }
    ExtraBossObj_DrawAll();
    ExtraBoss_DrawRings();
}
