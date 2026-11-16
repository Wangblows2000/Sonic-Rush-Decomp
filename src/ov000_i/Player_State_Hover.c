#include <nitro/types.h>

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
    u8 pad1C[0x20 - 0x1c];
    u32 unk20;
} UnkCfg_02348974;

extern UnkCfg_02348974 gGameState;
void EffBoostJet_Spawn(u8 *w);
void Player_SettleAirRotation(u8 *w);
void Player_ApplyAirInput(u8 *w);
void Player_LoadPhysicsParams(u8 *w);
void Player_Action_Fall(u8 *w);
void Player_Action_LandOnGround(u8 *w, s32 a);

void Player_State_Hover(u8 *w) {
    if (!(*(u32 *)(w + 0x330) & 0x1000) && !(gGameState.unk20 & 7) && (gGameState.unk18 & 1))
        EffBoostJet_Spawn(w);
    if (*(s16 *)(w + 0x3e0) != 0)
        (*(s16 *)(w + 0x3e0))--;
    Player_SettleAirRotation(w);
    Player_ApplyAirInput(w);
    if (!(*(u16 *)(w + 0x420) & 0x100) || *(s16 *)(w + 0x3e0) == 0) {
        Player_LoadPhysicsParams(w);
        *(s16 *)(w + 0x34) = *(s16 *)(w + 0x2e);
        *(u16 *)(w + 0x5c) |= 0x400;
        Player_Action_Fall(w);
        return;
    }
    if (!(*(u32 *)(w + 0x58) & 1))
        return;
    Player_LoadPhysicsParams(w);
    Player_Action_LandOnGround(w, 0);
    (*(void (**)(u8 *))(w + 0x334))(w);
}
