/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gGameState[];
s32 BossF_LoadPalette();

void BossFPartD8_BlinkPalette(s32 p0) {
    *(u16 *)((u8 *)p0 + 0x440) = (*(u16 *)((u8 *)p0 + 0x440) + 0x1);
    if ((*(u16 *)((u8 *)p0 + 0x440) & 3) != 0) {
        return;
    }
    if ((*(u16 *)((u8 *)p0 + 0x440) & 0x8000) != 0) {
        BossF_LoadPalette(*(u32 *)gGameState, 0);
    } else {
        BossF_LoadPalette(*(u32 *)gGameState, 1);
    }
    *(u16 *)((u8 *)p0 + 0x440) = (*(u16 *)((u8 *)p0 + 0x440) & ~3);
    *(u16 *)((u8 *)p0 + 0x440) = (*(u16 *)((u8 *)p0 + 0x440) ^ 0x8000);
}
