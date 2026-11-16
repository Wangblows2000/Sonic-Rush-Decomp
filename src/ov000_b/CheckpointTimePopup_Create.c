#include <ov000_b/stageobj.h>

extern void *gActComArchive;
extern UnkStruct_EffObj *data_ov000_0236a978;
extern u8 data_ov000_02367fd0[];

void CheckpointTimePopup_Destructor(void);
void CheckpointTimePopup_Draw(void);
void CheckpointTimePopup_State_Main(void);
s32 Hud_Unk_ov000_0233766c(s32 a);
void AnimatorSprite_ProcessAnimation(UnkStruct_Spr *s, s32 a, s32 b);
void Hud_FramesToTimeDigits(s32 *out, s32 n);

void CheckpointTimePopup_Create(s32 unused, s32 n) {
    UnkStruct_Task *task;
    UnkStruct_EffObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, CheckpointTimePopup_Destructor, 0, 0xff, 4, sizeof(UnkStruct_EffObj));
    if (task == OS_GetArenaLo(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_EffObj));
    data_ov000_0236a978 = w;
    StageObj_Load2dBAC(w, data_ov000_02367fd0, 0, NULL, gActComArchive);
    w->spr0.unk34 = Hud_Unk_ov000_0233766c(0);
    w->spr1.unk34 = Hud_Unk_ov000_0233766c(1);
    w->spr0.unk44 = 1;
    w->spr1.unk44 = 1;
    w->spr0.unk2C |= 0x10;
    w->spr1.unk2C |= 0x10;
    StageObj_SetOAMOrder(w, 3);
    StageObj_SetOAMPriority(w, 0);
    w->unk54 |= 0x10;
    w->unk5C |= 0x1000;
    w->unk5C |= 1;
    w->unk5C |= 0x4000;
    AnimatorSprite_ProcessAnimation(&w->spr0, 0, 0);
    AnimatorSprite_ProcessAnimation(&w->spr1, 0, 0);
    w->unk70 = CheckpointTimePopup_Draw;
    w->unk10 = 0xf800;
    w->unk14 = 0x1400;
    w->unk68 = CheckpointTimePopup_State_Main;
    w->unkC = 0;
    Hud_FramesToTimeDigits(&w->unk8, n);
    w->unk58 |= 0x2100;
    w->unk54 |= 2;
    w->unk5C |= 0x80;
}
