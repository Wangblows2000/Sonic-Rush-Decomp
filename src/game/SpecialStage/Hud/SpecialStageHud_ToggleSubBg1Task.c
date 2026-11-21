#include <nitro/types.h>

extern u8 *gSpecialStageHud;

void SpecialStageHud_ToggleSubBg1Task(void) {
    s32 v = *(s32 *)(gSpecialStageHud + 0xbd4);
    if (v < 0x3e) {
        u32 plane = (*(vu32 *)0x04001000 & 0x1f00) >> 8;
        *(vu32 *)0x04001000 = (*(vu32 *)0x04001000 & ~0x1f00) | ((plane & ~2) << 8);
    } else if (v == 0x3e) {
        u32 plane = (*(vu32 *)0x04001000 & 0x1f00) >> 8;
        *(vu32 *)0x04001000 = (*(vu32 *)0x04001000 & ~0x1f00) | ((plane | 2) << 8);
    }
}
