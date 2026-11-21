#include <game/Stage/stageobj.h>

typedef struct {
    u8 pad[4];
    s32 unk4;
} UnkStruct_02369ac0;

typedef struct {
    u8 pad0[0x18];
    u16 unk18;
    u8 pad1A[2];
    s32 unk1C;
    u8 pad20[4];
    s32 unk24;
} UnkStruct_022c4534;

typedef struct {
    UnkStruct_StageObj base;
    UnkStruct_Coll coll20C;
    u8 pad238[0x434 - 0x238];
    u16 unk434;
    u16 unk436;
    u8 pad438[0x440 - 0x438];
    u16 unk440;
} UnkStruct_022f2444;

extern UnkStruct_02369ac0 gBossStatus;
extern UnkStruct_022c4534 gStageMapCollision;

void BossFPartD8_DisableHitboxes(UnkStruct_022f2444 *s);
void ObjRect_SetBox3D(UnkStruct_Coll *c, s16 a, s16 b, s16 d, s16 e, s16 f, s16 g);
void EffEnemyRoomFix_Create(s32 x, s16 y, s32 flip, s32 c);
void BossFPartD8_State_HandRideStartWait(void);

void BossFPartD8_Action_StartHandRide(UnkStruct_022f2444 *s) {
    UnkStruct_Coll *c;

    s->base.unk1B8 = s;
    BossFPartD8_DisableHitboxes(s);
    c = &s->coll20C;
    c->unkA |= 4;
    ObjRect_SetBox3D(c, -0x60, -0x100, 0, 0, -0x7d00, 0x7d00);
    s->unk434 = 0;
    s->unk436 = 0;
    gStageMapCollision.unk1C = 0;
    gStageMapCollision.unk24 = gStageMapCollision.unk18 << 6;
    EffEnemyRoomFix_Create(0xb4, 0x30, 0, 0xb4);
    s->unk440 = 0;
    gBossStatus.unk4 = 0x33;
    s->base.unk68 = BossFPartD8_State_HandRideStartWait;
}
