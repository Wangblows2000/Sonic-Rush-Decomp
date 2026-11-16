/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 SpecialStageCamera_SetMode();
s32 SpecialStagePlayer_ChangeState();
s32 SpecialStageMap_SetScrollSpeed();

void func_ov001_02311968(u8 *p0) {
    *(u32 *)((u8 *)p0 + 0x74) = 0xb4;
    SpecialStageCamera_SetMode(0, 0xb4);
    SpecialStagePlayer_ChangeState(p0, 2);
    *(u32 *)((u8 *)p0 + 0x120) = 0;
    s32 t0 = *(u16 *)((u8 *)p0 + 0x16);
    *(u32 *)((u8 *)p0 + 0x134) = t0;
    SpecialStageMap_SetScrollSpeed(0x3000, t0);
}
