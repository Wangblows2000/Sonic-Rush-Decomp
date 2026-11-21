/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 BossFPartD8_DisableHitboxes();
extern u8 gGameState[];
s32 BossFPartD8_LoadModelPalettesB();
s32 BossFPartD8_LoadModelPalettesA();
extern u8 gBossStatus[];
void BossFPartD8_State_HandSlamHurt();

void BossFPartD8_Action_HandSlamHurt(u8 *p0) {
    *(u16 *)((u8 *)p0 + 0x2e) = 0;
    BossFPartD8_DisableHitboxes(p0, 0);
    BossFPartD8_LoadModelPalettesB(*(u32 *)gGameState, 0);
    BossFPartD8_LoadModelPalettesA(*(u32 *)gGameState, 0);
    *(u32 *)((u8 *)gBossStatus + 0x4) = 0x39;
    *(u32 *)((u8 *)p0 + 0x68) = (u32)BossFPartD8_State_HandSlamHurt;
}
