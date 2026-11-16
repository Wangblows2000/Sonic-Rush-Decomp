#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x10C];
    u8 unk10C;
    u8 pad10D[0x114 - 0x10D];
    s32 unk114;
} UnkStruct_Mdl960;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x960 - 0x20C];
    UnkStruct_Mdl960 unk960;
    u8 pad[0xD08 - 0x960 - sizeof(UnkStruct_Mdl960)];
    void *unkD08;
    u8 padD0C[0xF4A - 0xD0C];
    s16 unkF4A;
    u8 padF4C[0xF58 - 0xF4C];
    s32 unkF58;
} UnkStruct_Boss;

void Boss6_Unk_ov000_022e5c1c(UnkStruct_Boss *w);
void Model_SetAnimation(void *s, s32 a, void *b, s32 c, s32 d);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void Boss6_State_Unk_ov000_022e4298(void);

void Boss6_State_Unk_ov000_022e434c(UnkStruct_Boss *w) {
    UnkStruct_Mdl960 *m = &w->unk960;

    Boss6_Unk_ov000_022e5c1c(w);
    w->base.unk10 += w->unkF58;
    if (w->unkF58 != 0)
        return;
    if (w->unkF4A != 0)
        return;
    w->base.unk68 = Boss6_State_Unk_ov000_022e4298;
    Model_SetAnimation(m, 0, w->unkD08, 7, 0);
    m->unk10C &= ~4;
    m->unk114 = 0x1000;
    Sound_PlaySeqArc(0, 0, 0x84);
}
