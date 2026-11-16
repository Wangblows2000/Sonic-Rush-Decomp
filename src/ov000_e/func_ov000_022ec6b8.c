#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x214 - 0x20C];
    UnkStruct_StageObj *unk214;
    UnkStruct_StageObj *unk218;
    UnkStruct_StageObj *unk21C;
} UnkStruct_022ec6b8;

typedef struct {
    u8 pad[0x114];
    s32 unk114;
} UnkStruct_124;

extern UnkStruct_StageObj *gPlayers;

void func_ov000_023491f8(UnkStruct_StageObj *p, s32 a, s32 b);
void func_ov000_022ee198(UnkStruct_StageObj *s, s32 st);
void func_ov000_022e9b70(UnkStruct_StageObj *s, s32 a, s32 b);
void BossEffect_CreateStrikeFin(s32 a, s32 x, s32 y, s32 z);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void Boss7_State_Unk_ov000_022ebe64(void);
void Boss7_State_Unk_ov000_022ec568(void);

void func_ov000_022ec6b8(UnkStruct_022ec6b8 *w) {
    UnkStruct_StageObj *p = gPlayers;
    UnkStruct_StageObj *b = w->unk214;

    func_ov000_023491f8(p, -0x300, -0x600);
    w->unk21C->unk68 = Boss7_State_Unk_ov000_022ebe64;
    w->unk21C = NULL;
    b->unk2E = -0x200;
    func_ov000_022ee198(b, 5);
    func_ov000_022e9b70(b, 0, 0);
    ((UnkStruct_124 *)b->unk124)->unk114 = 0x1000;
    b->unk5C &= ~4;
    w->unk218->unk68 = Boss7_State_Unk_ov000_022ebe64;
    w->unk218 = NULL;
    BossEffect_CreateStrikeFin(1, (b->unk10 + p->unk10) >> 1, (b->unk14 + p->unk14) >> 1, ((b->unk18 + p->unk18) >> 1) + 0x1000);
    Sound_PlaySeqArc(0, 0, 0x9b);
    w->base.unk68 = Boss7_State_Unk_ov000_022ec568;
}
