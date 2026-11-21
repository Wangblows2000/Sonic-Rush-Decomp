#include <game/Stage/stageobj_alt.h>

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366f18[], data_ov000_0236a23c[], data_ov000_023661c0[], data_ov000_023661a4[];

void Saucer_Destructor(void);
void Saucer_Action_Idle(UnkStruct_StageObj *w);

UnkStruct_StageObj *Saucer_Create(s32 a, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_StageObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, Saucer_Destructor, 0, 0xf6, 2, sizeof(UnkStruct_StageObj));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_StageObj));
    MapObject_Init(w, a, b, c);
    StageObj_Load2dBAC(w, data_ov000_02366f18, 0xffff, data_ov000_0236a23c, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0x3c);
    StageObj_SetOAMOrder(w, 0x14);
    w->spr0.unk2C |= 0x200;
    w->spr1.unk2C |= 0x200;
    w->unk1B8 = w;
    w->unk1C8 = data_ov000_023661c0;
    w->unk1D0 = data_ov000_023661a4;
    w->unk1D8 = 5;
    w->unk1DA = 2;
    w->unk1D4 = -0x13;
    w->unk1D6 = 0xa;
    w->unk1C4 |= 1;
    StageObj_SetMapHitbox(w, -0x15, -0x17, 0x15, 0x14);
    w->unk58 |= 0x2000;
    w->unk58 |= 0x40;
    w->unk58 |= 0x8000;
    w->unk58 |= 0x4000;
    w->unk54 |= 2;
    w->unk5C |= 0x100;
    w->unk1DC = Sound_AllocHandle();
    Saucer_Action_Idle(w);
    return w;
}
