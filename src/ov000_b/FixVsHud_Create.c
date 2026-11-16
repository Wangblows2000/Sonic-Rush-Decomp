#include <ov000_b/stageobj.h>

typedef struct {
    UnkStruct_EffObj base;
    UnkStruct_Spr s0;
    UnkStruct_Spr s1;
} UnkStruct_0232dc20;

extern void *gActComArchive;
extern u8 data_ov000_023680d8[], data_ov000_0236a9a8[], data_ov000_023680ec[];

void FixVs_Destructor(void);
void FixVsHud_Draw(void);

void FixVsHud_Create(void) {
    UnkStruct_Task *task;
    UnkStruct_0232dc20 *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, FixVs_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_0232dc20));
    if (task == OS_GetArenaLo(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0232dc20));
    StageObj_Load2dBAC(w, data_ov000_023680d8, 0xffff, data_ov000_0236a9a8, gActComArchive);
    w->base.spr0.unk44 = 1;
    w->base.spr1.unk44 = 1;
    StageObj_SetOAMOrder(w, 3);
    StageObj_SetOAMPriority(w, 0);
    ObjAction_Load2dBAC(&w->s0, &w->s1, data_ov000_023680ec, 0xffff, data_ov000_0236a9a8, gActComArchive);
    SpritePair_SetOAMOrder(&w->s0, &w->s1, 3);
    SpritePair_SetOAMPriority(&w->s0, &w->s1, 0);
    w->s0.unk44 = w->base.spr0.unk44;
    w->s1.unk44 = w->base.spr0.unk44;
    w->s0.unk2C |= 0x200;
    w->s1.unk2C |= 0x200;
    AnimatorSprite_SetAnimation(&w->s0, 2);
    AnimatorSprite_SetAnimation(&w->s1, 2);
    w->base.unk70 = FixVsHud_Draw;
    w->base.unk10 = 0xc800;
    w->base.unk14 = 0x400;
    w->base.unk5C |= 1;
    w->base.unk5C |= 0x80;
    w->base.unk58 |= 0x8000100;
    w->base.unk54 |= 0x12;
}
