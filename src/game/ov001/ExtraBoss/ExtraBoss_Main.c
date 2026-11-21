/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gExtraBossWork[];

void ExtraBoss_Main(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gExtraBossWork + 0xd4);
    if (t0 == 0) {
        return;
    }
    ((s32 (*)())t0)(t0);
}
