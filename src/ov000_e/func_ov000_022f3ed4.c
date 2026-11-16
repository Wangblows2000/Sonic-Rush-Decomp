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

void func_ov000_022f4ce4(UnkStruct_022f3ed4 *s, s32 a, s32 b, s32 c);
void func_ov000_022f9044(UnkStruct_022f3ed4 *s);
void func_ov000_022f4cbc(UnkStruct_022f3ed4 *s);
void func_ov000_022f9188(void);
void BossFPartD8_State_Unk_ov000_022f3e9c(void);

void func_ov000_022f3ed4(UnkStruct_022f3ed4 *s) {
    func_ov000_022f4ce4(s, 6, 0x1000, 1);
    func_ov000_022f9044(s);
    func_ov000_022f4cbc(s);
    s->base.unk5C |= 1;
    func_ov000_022f9188();
    gStageMapCollision.unk1C = 0;
    gStageMapCollision.unk24 = 0x340;
    s->unk434 = (Rand() & 0x3f) + 0x1e;
    gBossStatus.unk4 = 0x2f;
    s->base.unk68 = BossFPartD8_State_Unk_ov000_022f3e9c;
}
