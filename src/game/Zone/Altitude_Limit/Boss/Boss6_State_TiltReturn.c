#include <game/Stage/stageobj.h>

typedef struct {
    u8 pad0[0x10C];
    u8 unk10C;
    u8 pad10D[0x114 - 0x10D];
    s32 unk114;
} UnkStruct_Mdl960;

typedef struct {
    UnkStruct_StageObj base;
    u32 unk20C;
    u8 pad210[0x960 - 0x210];
    UnkStruct_Mdl960 unk960;
    u8 pad[0xD08 - 0x960 - sizeof(UnkStruct_Mdl960)];
    void *unkD08;
    u8 padD0C[0xD54 - 0xD0C];
    s32 unkD54;
    s32 unkD58;
    u8 padD5C[0xECA - 0xD5C];
    u16 unkECA;
    u8 padECC[0xF68 - 0xECC];
    s16 unkF68;
    u8 padF6A[2];
    s16 unkF6C;
} UnkStruct_Boss;

void Boss6_ApplyTiltToPlayer(UnkStruct_Boss *w);
void Boss6_SpawnDebrisTimed(UnkStruct_Boss *w);
void Boss6_Unk_ov000_022e1f10(UnkStruct_Boss *w);
void Model_SetAnimation(void *s, s32 a, void *b, s32 c, s32 d);
void Boss6_EndTiltCamera(UnkStruct_Boss *w);
void Boss6_State_TiltEnd(void);

void Boss6_State_TiltReturn(UnkStruct_Boss *w) {
    UnkStruct_Mdl960 *m = &w->unk960;

    Boss6_ApplyTiltToPlayer(w);
    Boss6_SpawnDebrisTimed(w);
    Boss6_Unk_ov000_022e1f10(w);
    if (w->unkF68 != w->unkF6C)
        return;
    w->base.unk68 = Boss6_State_TiltEnd;
    Model_SetAnimation(m, 0, w->unkD08, 0xb, 0);
    m->unk114 = 0x1000;
    w->unkECA &= ~4;
    w->unkECA |= 0x1000;
    w->unk20C &= ~8;
    Boss6_EndTiltCamera(w);
    w->unkD54 = -0x17ff;
    w->unkD58 = 0x17ff;
}
