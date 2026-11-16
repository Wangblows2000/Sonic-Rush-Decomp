/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 BossFPartD8_SetAnimation();
s32 BossFPartD8_InitHitboxes();
s32 BossFPartD8_DisableHitboxes();
s32 BossF_InitCamera();
extern u8 gBossStatus[];
void BossFPartD8_State_HurtInSky();

void BossFPartD8_Action_HurtInSky(u8 *p0) {
    BossFPartD8_SetAnimation(p0, 0x24, 0x1000, 0);
    BossFPartD8_InitHitboxes(p0);
    BossFPartD8_DisableHitboxes(p0);
    *(u32 *)((u8 *)p0 + 0x1b8) = 0;
    BossF_InitCamera(0);
    *(u16 *)((u8 *)p0 + 0x434) = 0;
    *(u32 *)((u8 *)gBossStatus + 0x4) = 0x3a;
    *(u32 *)((u8 *)p0 + 0x68) = (u32)BossFPartD8_State_HurtInSky;
}
