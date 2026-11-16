/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gGameState[];
s32 EffBlazeSpinFlame_Create();

void EffBlazeSpinFlame_Spawn(u8 *p0) {
    if (*(u8 *)((u8 *)p0 + 0x325) != 0) {
        return;
    }
    if ((u32)*(u8 *)((u8 *)gGameState + 0x1c) >= (u32)9) {
        return;
    }
    if ((u32)*(u8 *)((u8 *)gGameState + 0x1d) >= (u32)2) {
        return;
    }
    if ((*(u32 *)((u8 *)p0 + 0x330) & 0x1000) != 0) {
        return;
    }
    EffBlazeSpinFlame_Create(p0, 0, 8);
}
