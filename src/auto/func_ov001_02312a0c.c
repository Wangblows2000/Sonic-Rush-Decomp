/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 SpecialStageMap_SetScrollSpeed();
s32 SpecialStagePlayer_ChangeState();

void func_ov001_02312a0c(u8 *p0, s32 p1) {
    SpecialStageMap_SetScrollSpeed(0);
    SpecialStagePlayer_ChangeState(p0, 0xa);
    *(u16 *)((u8 *)p0 + 0x16) = 0;
    *(u16 *)((u8 *)p0 + 0x14c) = 0;
    *(u16 *)((u8 *)p0 + 0x14e) = 1;
    *(u32 *)((u8 *)p0 + 0x128) = 1;
    *(u16 *)((u8 *)p0 + 0x150) = p1;
}
