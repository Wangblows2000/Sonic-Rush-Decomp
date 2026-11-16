#include <ov000_c/stageobj.h>

typedef struct {
    u32 unk0;
    u8 pad[0x10 - 4];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

typedef struct {
    u8 pad[8];
    u16 unk8;
} UnkStruct_02369ac0;

typedef struct {
    u8 pad[0x20C];
    u32 unk20C;
} UnkStruct_Mgr;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x314 - 0x20C];
    UnkStruct_Coll coll314;
    u8 pad340[0x430 - 0x340];
    u16 unk430;
    u8 pad432[0x438 - 0x432];
    u16 unk438;
    u16 unk43A;
} UnkStruct_022f28b4;

extern UnkStruct_022c4560 gGameState;
extern UnkStruct_02369ac0 gBossStatus;
extern UnkStruct_Mgr *gBossF;
extern u16 data_ov000_02364344[][8];

void BossFPartD8_Unk_ov000_022f4cbc(UnkStruct_022f28b4 *s);
void ObjRect_SetBox3D(UnkStruct_Coll *c, s16 a, s16 b, s16 d, s16 e, s16 f, s16 g);
void BossFPartD8_State_Unk_ov000_022f2724(UnkStruct_022f28b4 *s);

static inline s32 GetIdxB(void) {
    BOOL b = TRUE;
    if (gGameState.unk10 != 2 && gGameState.unk10 != 3)
        b = FALSE;
    if (b)
        return 1;
    return gGameState.unk14;
}

void BossFPartD8_State_Unk_ov000_022f28b4(UnkStruct_022f28b4 *s) {
    UnkStruct_Mgr *m = gBossF;

    BossFPartD8_Unk_ov000_022f4cbc(s);
    s->coll314.unkA |= 4;
    ObjRect_SetBox3D(&s->coll314, 0x10, -0x80, 0x70, 0x38, -0x7d00, 0x7d00);
    s->unk438 = data_ov000_02364344[GetIdxB()][gBossStatus.unk8 - 1];
    s->unk43A = 0;
    s->unk430 = 0;
    m->unk20C |= 0x40;
    s->base.unk1B8 = s;
    s->base.unk68 = BossFPartD8_State_Unk_ov000_022f2724;
    ((void (*)(UnkStruct_022f28b4 *))s->base.unk68)(s);
}
