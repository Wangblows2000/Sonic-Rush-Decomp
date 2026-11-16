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
    u8 padD0C[0xD60 - 0xD0C];
    UnkStruct_Coll colls[11];
    u8 padF44[0xF4C - 0xD60 - 11 * 0x2C];
    s16 unkF4C;
    u8 padF4E[0xF68 - 0xF4E];
    u16 unkF68;
    u16 unkF6A;
    u16 unkF6C;
    u8 padF6E[0x1118 - 0xF6E];
    s32 unk1118;
} UnkStruct_Boss;

void Model_SetAnimation(void *s, s32 a, void *b, s32 c, s32 d);
void Boss6_UpdateFlashPalettes(s32 a);
void Boss6_State_HurtWaitSelectAttack(void);

void Boss6_State_HurtRecover(UnkStruct_Boss *w) {
    UnkStruct_Mdl960 *m = &w->unk960;

    if (m->unk10C & 2) {
        w->base.unk68 = Boss6_State_HurtWaitSelectAttack;
        Model_SetAnimation(m, 0, w->unkD08, 0x17, 0);
        m->unk10C &= ~4;
        m->unk114 = 0x1000;
        w->base.unk10 = w->unk1118;
        w->unkF4C = 0;
        Boss6_UpdateFlashPalettes(4);
        w->unkF68 = 0x549f;
        w->unkF6A = 0;
        w->unkF6C = 0x549f;
        return;
    }
    w->unk1118++;
    Boss6_UpdateFlashPalettes(w->unk1118);
}
