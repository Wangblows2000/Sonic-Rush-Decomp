/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gExtraBossPlayerManager[];
s32 ExtraBossObj_Destroy();

void ExtraBossObj_DestroyPlayer(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gExtraBossPlayerManager + 0x4);
    if (t0 == 0) {
        return;
    }
    ExtraBossObj_Destroy(t0);
}
