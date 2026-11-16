/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 BossF_UpdateCameraShake();
extern u8 gGameState[];
s32 BossFPartD8_LoadModelPalettesB();
s32 BossFPartD8_LoadModelPalettesA();
s32 BossFPartD8_DisableHitboxes();
void BossFPartD8_State_DoublePressEnd();

void BossFPartD8_State_DoublePressVulnerable(u8 *p0) {
    BossF_UpdateCameraShake(((u8 *)p0 + 0x436), 3, 7);
    *(u16 *)((u8 *)p0 + 0x43e) = (*(u16 *)((u8 *)p0 + 0x43e) + 0x1);
    if ((*(u16 *)((u8 *)p0 + 0x43e) & 3) == 0) {
        if ((*(u16 *)((u8 *)p0 + 0x43a) & 0x8000) != 0) {
            BossFPartD8_LoadModelPalettesB(*(u32 *)gGameState, 0);
            BossFPartD8_LoadModelPalettesA(*(u32 *)gGameState, 0);
        } else {
            BossFPartD8_LoadModelPalettesB(*(u32 *)gGameState, 1);
            BossFPartD8_LoadModelPalettesA(*(u32 *)gGameState, 1);
        }
        *(u16 *)((u8 *)p0 + 0x43a) = (*(u16 *)((u8 *)p0 + 0x43a) & ~3);
        *(u16 *)((u8 *)p0 + 0x43a) = (*(u16 *)((u8 *)p0 + 0x43a) ^ 0x8000);
    }
    s32 t0 = *(u16 *)((u8 *)p0 + 0x438);
    if (t0 != 0) {
        *(u16 *)((u8 *)p0 + 0x438) = (t0 - 0x1);
    }
    if (*(u16 *)((u8 *)p0 + 0x438) != 0) {
        return;
    }
    if ((*(u16 *)((u8 *)p0 + 0x43a) & 0x8000) != 0) {
        return;
    }
    BossFPartD8_DisableHitboxes(p0, *(u16 *)((u8 *)p0 + 0x438));
    *(u32 *)((u8 *)p0 + 0x68) = (u32)BossFPartD8_State_DoublePressEnd;
}
