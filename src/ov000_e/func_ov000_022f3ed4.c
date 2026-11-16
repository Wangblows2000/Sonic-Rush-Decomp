#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad[4];
    s32 unk4;
} UnkStruct_02369ac0;

typedef struct {
    u8 pad0[0x1C];
    s32 unk1C;
    u8 pad20[4];
    s32 unk24;
} UnkStruct_022c4534;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x434 - 0x20C];
    u16 unk434;
} UnkStruct_022f3ed4;

extern UnkStruct_02369ac0 gBossStatus;
extern UnkStruct_022c4534 gStageMapCollision;

void BossFPartD8_SetAnimation(UnkStruct_022f3ed4 *s, s32 a, s32 b, s32 c);
void BossFPartD8_InitHitboxes(UnkStruct_022f3ed4 *s);
void func_ov000_022f4cbc(UnkStruct_022f3ed4 *s);
void BossF_InitCamera(void);
void BossFPartD8_State_Unk_ov000_022f3e9c(void);

void func_ov000_022f3ed4(UnkStruct_022f3ed4 *s) {
    BossFPartD8_SetAnimation(s, 6, 0x1000, 1);
    BossFPartD8_InitHitboxes(s);
    func_ov000_022f4cbc(s);
    s->base.unk5C |= 1;
    BossF_InitCamera();
    gStageMapCollision.unk1C = 0;
    gStageMapCollision.unk24 = 0x340;
    s->unk434 = (Rand() & 0x3f) + 0x1e;
    gBossStatus.unk4 = 0x2f;
    s->base.unk68 = BossFPartD8_State_Unk_ov000_022f3e9c;
}
