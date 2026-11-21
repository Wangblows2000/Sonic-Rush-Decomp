#include <ov000_c/stageobj.h>

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
    u8 padD0C[0xD5E - 0xD0C];
    u16 unkD5E;
    u8 padD60[0xE72 - 0xD60];
    u16 unkE72;
    u8 padE74[0xEF6 - 0xE74];
    u16 unkEF6;
    u8 padEF8[0xF4C - 0xEF8];
    s16 unkF4C;
    u8 padF4E[0xF50 - 0xF4E];
    s32 unkF50;
    u8 padF54[0xF66 - 0xF54];
    u16 unkF66;
    u16 unkF68;
    u16 unkF6A;
    u16 unkF6C;
    u8 padF6E[0x1118 - 0xF6E];
    s32 unk1118;
} UnkStruct_Boss;

void Model_SetAnimation(void *s, s32 a, void *b, s32 c, s32 d);
void Boss6_SetCameraAngle(UnkStruct_Boss *w, s32 a);
void Boss6_State_EdgeJumpLand(void);

void Boss6_State_EdgeJumpLeap(UnkStruct_Boss *w) {
    UnkStruct_Mdl960 *m = &w->unk960;

    if (!(m->unk10C & 2))
        return;
    w->base.unk68 = Boss6_State_EdgeJumpLand;
    Model_SetAnimation(m, 0, w->unkD08, 0xe, 0);
    m->unk10C &= ~4;
    m->unk114 = 0x1000;
    w->unkF50 = 0;
    w->unkF66 = 0;
    w->unkF68 = 0;
    w->unkF6A = 0;
    w->unkF6C = 0;
    w->unk1118 = w->base.unk10;
    if (Rand() & 1) {
        w->base.unk10 = 0x1800;
        if (w->unkD5E != 0)
            w->unkF4C = 0;
        else
            w->unkF4C = -0x8000;
    } else {
        w->base.unk10 = 0x32800;
        if (w->unkD5E != 0)
            w->unkF4C = -0x8000;
        else
            w->unkF4C = 0;
    }
    w->unkE72 |= 4;
    w->unkE72 &= ~0x1000;
    w->unkEF6 |= 4;
    w->unkEF6 &= ~0x1000;
    w->unk20C |= 0x10;
    Boss6_SetCameraAngle(w, w->unkF4C);
}
