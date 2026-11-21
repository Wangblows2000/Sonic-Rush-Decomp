#include <ov000_b/stageobj.h>

extern void *data_ov000_02369ba0;
extern u8 data_ov000_02366c90[], data_ov000_0236a0b4[], data_ov000_0236a0bc[], data_ov000_023661c0[], data_ov000_023661a4[];

void MapObject_Destructor(void);
void LandUnrest_Action_Init(UnkStruct_StageObj *w);

UnkStruct_StageObj *LandUnrest_Create(s32 a, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_StageObj *w;
    vu16 zero;

    task = Task_Create(StageObj_Main, MapObject_Destructor, 0, 0xf6, 2, sizeof(UnkStruct_StageObj));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_StageObj));
    MapObject_Init(w, a, b, c);
    StageObj_Load2dBAC(w, data_ov000_02366c90, 0, data_ov000_0236a0b4, data_ov000_02369ba0);
    StageObj_AllocSpritePalette(w, 0, 0x1d);
    StageObj_SetSharedSpriteVram(w, 0xffff, data_ov000_0236a0bc);
    w->spr0.unk46 = 0x14;
    w->spr1.unk46 = 0x14;
    w->spr0.unk2C |= 0x200;
    w->spr1.unk2C |= 0x200;
    w->unk1B8 = w;
    w->unk1C8 = data_ov000_023661c0;
    w->unk1D0 = data_ov000_023661a4;
    w->unk1D8 = 8;
    w->unk1DA = 2;
    w->unk1D4 = -0x21;
    w->unk1D6 = -0x1a;
    w->unk58 |= 0x100;
    w->unk5C |= 0x100;
    w->unk54 |= 2;
    LandUnrest_Action_Init(w);
    return w;
}
