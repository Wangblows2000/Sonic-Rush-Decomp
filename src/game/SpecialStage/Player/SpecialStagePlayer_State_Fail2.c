/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 SpecialStageMap_GetScrollSpeed();
extern u8 gSpecialStagePlayerResources[];
s32 Model_SetResource();
s32 Model_SetAnimation();
void SpecialStagePlayer_State_Fail3();
s32 SpecialStagePlayer_GetSpeedAnimRate();

void SpecialStagePlayer_State_Fail2(u8 *p0) {
    s32 t0 = SpecialStageMap_GetScrollSpeed();
    if (t0 < 0x1800) {
        *(u8 *)((u8 *)*(u32 *)((u8 *)p0 + 0xac) + 0x10c) = (*(u8 *)((u8 *)*(u32 *)((u8 *)p0 + 0xac) + 0x10c) & ~4);
        s32 t1 = *(u32 *)((u8 *)p0 + 0xac);
        if ((*(u8 *)((u8 *)*(u32 *)((u8 *)p0 + 0xac) + 0x10c) & 2) == 0) {
            return;
        }
        Model_SetResource(t1, *(u32 *)((u8 *)*(u32 *)gSpecialStagePlayerResources + 0x13c), 0);
        Model_SetAnimation(*(u32 *)((u8 *)p0 + 0xac), 0, *(u32 *)((u8 *)*(u32 *)gSpecialStagePlayerResources + 0x140), 2, 0);
        Model_SetAnimation((*(u32 *)gSpecialStagePlayerResources + 0x4), 4, *(u32 *)((u8 *)*(u32 *)gSpecialStagePlayerResources + 0x144), 1, 0);
        *(u8 *)((u8 *)*(u32 *)((u8 *)p0 + 0xac) + 0x10c) = (*(u8 *)((u8 *)*(u32 *)((u8 *)p0 + 0xac) + 0x10c) | 4);
        *(u32 *)((u8 *)p0 + 0x22c) = (u32)SpecialStagePlayer_State_Fail3;
        return;
    }
    s32 t2 = SpecialStagePlayer_GetSpeedAnimRate(t0);
    *(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0xac) + 0x114) = t2;
}
