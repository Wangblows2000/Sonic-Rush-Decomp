#include <ov000_b/stageobj.h>

extern void *gActComArchive;
extern u8 data_ov000_02367fa8[];

void func_ov000_0232a764(void);
void func_ov000_0232a710(void);
s32 func_ov000_0233766c(s32 a);
void AnimatorSprite_ProcessAnimation(UnkStruct_Spr *s, s32 a, s32 b);

UnkStruct_EffObj *func_ov000_0232ab9c(UnkStruct_StageObj *arg, s32 x, s32 y, u32 idx) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    if (idx >= 10)
        return NULL;
    task = Task_Create(StageObj_Main, func_ov000_0232a764, 0, 0xa00, 4, sizeof(UnkStruct_EffObj));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    StageObj_Load2dBAC(w, data_ov000_02367fa8, 0, NULL, gActComArchive);
    w->spr0.unk34 = func_ov000_0233766c(0);
    w->spr1.unk34 = func_ov000_0233766c(1);
    w->spr0.unk34 += idx << 7;
    w->spr1.unk34 += idx << 7;
    w->spr0.unk44 = 1;
    w->spr1.unk44 = 1;
    w->spr0.unk2C |= 0x10;
    w->spr1.unk2C |= 0x10;
    w->spr0.unk2C |= 8;
    w->spr1.unk2C |= 8;
    StageObj_SetOAMOrder(w, 3);
    StageObj_SetOAMPriority(w, 0);
    w->unk54 |= 0x10;
    w->unk5C |= 0x1000;
    w->unk5C |= 1;
    AnimatorSprite_ProcessAnimation(&w->spr0, 0, 0);
    AnimatorSprite_ProcessAnimation(&w->spr1, 0, 0);
    w->unk2E = x << 8;
    w->unk30 = y << 8;
    w->unk10 = arg->unk10 + w->unk2E;
    w->unk14 = arg->unk14 + w->unk30;
    w->unk88 = arg;
    w->unk68 = func_ov000_0232a710;
    w->unkC = 0;
    w->unk8 = 0;
    w->unk58 |= 0x2100;
    w->unk54 |= 2;
    return w;
}
