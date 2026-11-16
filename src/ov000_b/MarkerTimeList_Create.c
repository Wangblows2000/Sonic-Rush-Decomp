#include <ov000_b/stageobj.h>

typedef struct {
    UnkStruct_EffObj base;
    u8 pad134[0x148 - 0x134];
} UnkStruct_0232b020;

extern void *gActComArchive;
extern UnkStruct_Task *gMarkerTimeListTask;
extern u8 data_ov000_02367fbc[];

void MarkerTimeList_Destructor(void);
void MarkerTimeList_Draw(void);
void MarkerTimeList_State_Idle(void);
s32 Hud_Unk_ov000_0233766c(s32 a);
void AnimatorSprite_ProcessAnimation(UnkStruct_Spr *s, s32 a, s32 b);

void MarkerTimeList_Create(void) {
    UnkStruct_Task *task;
    UnkStruct_0232b020 *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, MarkerTimeList_Destructor, 0, 0xff, 4, sizeof(UnkStruct_0232b020));
    if (task == OS_GetArenaLo(0))
        return;
    gMarkerTimeListTask = task;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0232b020));
    StageObj_Load2dBAC(w, data_ov000_02367fbc, 0, NULL, gActComArchive);
    w->base.spr0.unk34 = Hud_Unk_ov000_0233766c(0);
    w->base.spr1.unk34 = Hud_Unk_ov000_0233766c(1);
    w->base.spr0.unk44 = 1;
    w->base.spr1.unk44 = 1;
    w->base.spr0.unk2C |= 0x10;
    w->base.spr0.unk2C |= 0x80000;
    w->base.spr1.unk2C |= 0x10;
    StageObj_SetOAMOrder(w, 3);
    StageObj_SetOAMPriority(w, 0);
    w->base.unk54 |= 0x10;
    w->base.unk5C |= 0x1000;
    w->base.unk5C |= 1;
    w->base.unk5C |= 0x4000;
    AnimatorSprite_ProcessAnimation(&w->base.spr0, 0, 0);
    AnimatorSprite_ProcessAnimation(&w->base.spr1, 0, 0);
    w->base.unk70 = MarkerTimeList_Draw;
    w->base.unk10 = 0;
    w->base.unk14 = 0;
    w->base.unk68 = MarkerTimeList_State_Idle;
    w->base.unkC = 0;
    w->base.unk58 |= 0x2100;
    w->base.unk54 |= 2;
    w->base.unk5C |= 0x80;
}
