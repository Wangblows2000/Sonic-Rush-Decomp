/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gGameState[];
extern u8 gZoneFxUpdateTable[];
extern u8 gZoneFxWork[];

void ZoneFx_Update(void) {
    s32 t0 = *(u32 *)((u8 *)(*(u8 *)((u8 *)gGameState + 0x1c) * 0xc + (s32)gZoneFxUpdateTable) + (*(u8 *)((u8 *)gGameState + 0x1d) << 2));
    s32 t1 = *(u8 *)((u8 *)gGameState + 0x1c);
    if (t0 == 0) {
        return;
    }
    ((s32 (*)())t0)(*(u32 *)gZoneFxWork, t0, gZoneFxUpdateTable, t1);
}
