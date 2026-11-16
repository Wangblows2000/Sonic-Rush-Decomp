/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 BossFPartD8_DisableHitboxes();
extern u8 gGameState[];
s32 BossFPartD8_LoadModelPalettesA();
extern u8 gBossStatus[];
void BossFPartD8_State_PartDAThrowHurt();

void BossFPartD8_Action_PartDAThrowHurt(u8 *p0) {
    BossFPartD8_DisableHitboxes();
    BossFPartD8_LoadModelPalettesA(*(u32 *)gGameState, 0);
    *(u32 *)((u8 *)gBossStatus + 0x4) = 0x39;
    *(u32 *)((u8 *)p0 + 0x68) = (u32)BossFPartD8_State_PartDAThrowHurt;
}
