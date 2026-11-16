#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x218 - 0x20C];
    UnkStruct_Vec unk218;
    u8 pad224[0x35C - 0x224];
    UnkStruct_Vec unk35C;
} UnkStruct_022f66e8;

extern UnkStruct_StageObj *gPlayers;

void BossFPartDE_State_Unk_ov000_022f66d0(void);

void func_ov000_022f66e8(UnkStruct_022f66e8 *w) {
    w->base.unk10 = gPlayers->unk10;
    w->base.unk14 = 0x23800;
    w->base.unk18 = gPlayers->unk18;
    w->base.unk30 = 0xa00;
    Obj_SetVec(&w->unk218, w->base.unk10 - 0x8000, 0x30000, 0);
    w->unk218.x = w->unk218.x < 0 ? 0 : (w->unk218.x > 0x34000 ? 0x34000 : w->unk218.x);
    Obj_SetVec(&w->unk35C, w->base.unk10 + 0x8000, 0x30000, 0);
    w->unk35C.x = w->unk35C.x < 0 ? 0 : (w->unk35C.x > 0x34000 ? 0x34000 : w->unk35C.x);
    w->base.unk68 = BossFPartDE_State_Unk_ov000_022f66d0;
}
