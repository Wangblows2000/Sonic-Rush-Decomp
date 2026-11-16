#include <ov000_b/stageobj.h>

typedef struct {
    UnkStruct_EffObj base;
    s16 unk134[5];
    s16 unk13E;
    u32 unk140;
    s32 unk144;
    u32 unk148;
} UnkStruct_0232b898;

extern void *gActComArchive;
extern UnkStruct_Task *data_ov000_0236a980;
extern u8 data_ov000_02367fe4[];

void ScorePopup_Destructor(void);
void ScorePopup_Draw(void);
void ScorePopup_State_Main(void);
s32 Hud_Unk_ov000_0233766c(s32 a);
void AnimatorSprite_ProcessAnimation(UnkStruct_Spr *s, s32 a, s32 b);
void Hud_ToDecimalDigits(u32 *out, s32 a, s32 b);

void ScorePopup_Create(UnkStruct_StageObj *arg, s32 n) {
    UnkStruct_Task *task;
    UnkStruct_0232b898 *w;
    vu16 zero;
    s16 i;

    if (data_ov000_0236a980 == NULL) {
        if (n == 0)
            return;
        task = Task_Create(StageObj_Main, ScorePopup_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_0232b898));
        if (task == OS_GetArenaLo(0))
            return;
        data_ov000_0236a980 = task;
        w = task->work;
        zero = 0;
        MIi_CpuClear16(zero, w, sizeof(UnkStruct_0232b898));
        StageObj_Load2dBAC(w, data_ov000_02367fe4, 0, NULL, gActComArchive);
        w->base.spr0.unk34 = Hud_Unk_ov000_0233766c(0);
        w->base.spr1.unk34 = Hud_Unk_ov000_0233766c(1);
        w->base.spr0.unk44 = 1;
        w->base.spr1.unk44 = 1;
        w->base.spr0.unk2C |= 0x10;
        w->base.spr1.unk2C |= 0x10;
        w->base.spr0.unk2C |= 0x208;
        w->base.spr1.unk2C |= 0x208;
        w->base.spr0.unk46 = 3;
        w->base.spr1.unk46 = 3;
        StageObj_SetOAMOrder(w, 3);
        StageObj_SetOAMPriority(w, 0);
        w->base.unk54 |= 0x10;
        w->base.unk5C |= 0x1000;
        w->base.unk5C |= 1;
        AnimatorSprite_ProcessAnimation(&w->base.spr0, 0, 0);
        AnimatorSprite_ProcessAnimation(&w->base.spr1, 0, 0);
        w->unk144 = n;
    } else {
        w = data_ov000_0236a980->work;
        if (n == 0) {
            w->unk148 = 0;
            w->unk144 = 0;
            w->unk140 = 0;
            return;
        }
    }
    for (i = 0; i < 5; i++)
        w->unk134[i] = i * 2 + 8;
    w->base.unk70 = ScorePopup_Draw;
    w->base.unk10 = arg->unk10;
    w->base.unk14 = arg->unk14;
    w->base.unk88 = arg;
    w->base.unk68 = ScorePopup_State_Main;
    w->base.unkC = 0;
    w->base.unk8 = 0;
    w->base.unk44 = 0;
    w->base.unk46 = 0;
    w->unk13E = 0;
    w->unk148 += n;
    if (w->unk144 == 0)
        w->unk144 = w->unk148;
    Hud_ToDecimalDigits(&w->unk140, w->unk144, 100000);
    if (w->unk148 > 2000)
        w->base.unk8 = 0x180;
    w->base.unk58 |= 0x2100;
    w->base.unk54 |= 2;
}
