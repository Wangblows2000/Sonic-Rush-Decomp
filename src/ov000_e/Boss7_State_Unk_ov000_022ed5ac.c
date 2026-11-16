#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x320 - 0x20C];
    s32 unk320;
} UnkStruct_022ed5ac_O;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x214 - 0x20C];
    UnkStruct_StageObj *unk214;
    UnkStruct_022ed5ac_O *unk218;
    UnkStruct_022ed5ac_O *unk21C;
} UnkStruct_022ed5ac;

extern UnkStruct_StageObj *gPlayers;

void Boss7_State_Unk_ov000_022ed428(void);

void Boss7_State_Unk_ov000_022ed5ac(UnkStruct_022ed5ac *w) {
    UnkStruct_StageObj *b = w->unk214;
    UnkStruct_StageObj *p = gPlayers;

    b->unk2E = -0x800;
    p->unk34 = 0x800;
    p->unk2E = 0x800;
    if (b->unk10 - p->unk10 > 0x2000)
        return;
    p->unk10 = ((b->unk10 + p->unk10) >> 1) - 0x1000;
    b->unk10 = ((b->unk10 + p->unk10) >> 1) + 0x1000;
    b->unk2E = 0;
    p->unk34 = 0;
    p->unk2E = 0;
    w->unk218->unk320 = 0x1000;
    w->unk21C->unk320 = 0x1000;
    w->base.unk68 = Boss7_State_Unk_ov000_022ed428;
}
