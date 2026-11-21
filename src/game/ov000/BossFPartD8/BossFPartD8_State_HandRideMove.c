#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x48];
    s32 unk48;
    s32 unk4C;
} UnkStruct_Cam;

typedef struct UnkStruct_022f1f70 {
    UnkStruct_StageObj base;
    u8 pad20C[0x434 - 0x20C];
    u16 unk434;
    u8 pad436[0x57C - 0x436];
    u8 unk57C;
} UnkStruct_022f1f70;

extern UnkStruct_Cam data_ov000_0236a48c;
extern UnkStruct_StageObj *gPlayers;

BOOL BossFPartD8_CheckKnockOffRider(UnkStruct_022f1f70 *s);
BOOL BossFPartD8_IsPlayerRiding(void);
BOOL BossFPartD8_IsPlayerCrouching(void);
void BossFPartD8_GetRidePosition(UnkStruct_022f1f70 *s, s32 *x, s32 *y, s32 c);
void BossFPartD8_State_HandRideStartHold(UnkStruct_022f1f70 *s);

void BossFPartD8_State_HandRideMove(UnkStruct_022f1f70 *s) {
    s32 x;
    s32 y;
    UnkStruct_Cam *cam;

    BossFPartD8_CheckKnockOffRider(s);
    if (BossFPartD8_IsPlayerRiding() && BossFPartD8_IsPlayerCrouching()) {
        cam = (UnkStruct_Cam *)(u32)&data_ov000_0236a48c;
        BossFPartD8_GetRidePosition(s, &x, &y, 0);
        cam->unk48 = ((x - gPlayers->unk10) << 4) + 0x14000;
        cam->unk4C = 0x32000 - ((y - gPlayers->unk14) << 4);
    }
    if (s->unk57C & 2)
        s->base.unk68 = BossFPartD8_State_HandRideStartHold;
}
