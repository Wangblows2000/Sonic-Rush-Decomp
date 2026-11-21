/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 SpecialStageMap_SetScrollSpeed();
s32 SpecialStageCamera_SetMode();
s32 Sound_PlaySeqArc();

void SpecialStagePlayer_StartDash(u8 *p0, u8 *p1) {
    SpecialStageMap_SetScrollSpeed(0x6000);
    *(u32 *)((u8 *)p0 + 0x144) = 0x3c;
    SpecialStageCamera_SetMode(3);
    *(u16 *)((u8 *)p0 + 0x16) = *(u16 *)((u8 *)p1 + 0x16);
    Sound_PlaySeqArc(0, 0x10, 0x17);
}
