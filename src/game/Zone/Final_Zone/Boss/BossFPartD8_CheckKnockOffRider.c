#include <game/Stage/stageobj.h>

typedef struct {
    u8 pad[0x21C];
    void *unk21C;
} UnkStruct_Mgr;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad[0x328 - 0x20c];
    s16 unk328;
} UnkStruct_Player;

extern UnkStruct_Player *gPlayers;
extern UnkStruct_Mgr *gBossF;

BOOL BossFPartD8_IsPlayerCrouching(void);
BOOL BossFPartD8_IsPlayerRiding(void);
void BossFPartD8_GetRidePosition(UnkStruct_StageObj *s, s32 *x, s32 *y, s32 c);
void Player_Action_Knockback(UnkStruct_Player *p, s16 a, s32 b);
void BossFPartD8_SetCameraFollowPlayer(void);

BOOL BossFPartD8_CheckKnockOffRider(UnkStruct_StageObj *s) {
    UnkStruct_Player *p = gPlayers;
    s32 x;
    s32 y;

    switch (p->unk328) {
    case 0xf:
    case 0x11:
    case 0x12:
        p->base.unk70 = gBossF->unk21C;
        gPlayers->base.unk5C |= 0x40;
        return FALSE;
    }
    if (!BossFPartD8_IsPlayerCrouching()) {
        if (BossFPartD8_IsPlayerRiding() || gPlayers->base.unk10 > 0x2c000) {
            BossFPartD8_GetRidePosition(s, &x, &y, 0);
            gPlayers->base.unk10 = x;
            gPlayers->base.unk14 = y;
            Player_Action_Knockback(gPlayers, (0x1a000 - gPlayers->base.unk10) >> 7, -0x800);
            BossFPartD8_GetRidePosition(s, &gPlayers->base.unk10, &gPlayers->base.unk14, 0);
            gPlayers->base.unk70 = gBossF->unk21C;
            gPlayers->base.unk5C |= 0x40;
            s->unk1B8 = NULL;
            BossFPartD8_SetCameraFollowPlayer();
            return TRUE;
        }
    }
    return FALSE;
}
