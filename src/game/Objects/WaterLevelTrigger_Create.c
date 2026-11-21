#include <game/Stage/stageobj_alt.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[2];
    u16 unk20E;
} UnkStruct_0231ba3c;

typedef struct {
    u32 unk0;
    u8 pad4[0x62 - 4];
    u16 unk62;
    u32 unk64;
    u8 pad68[0xC6 - 0x68];
    u16 unkC6;
} UnkStruct_0236aad8;

extern UnkStruct_0236aad8 gCamera;

void WaterLevelTrigger_Destructor(void);
void WaterLevelTrigger_State_ApplyOnce(UnkStruct_0231ba3c *w);
void WaterLevelTrigger_Action_InitBySide(UnkStruct_0231ba3c *w);

UnkStruct_0231ba3c *WaterLevelTrigger_Create(UnkStruct_ObjParam *p, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_0231ba3c *w;
    vu16 zero;

    if (p->unk4 & 4) {
        gCamera.unk62 = gCamera.unkC6 = (u8)p->unk6 * 10000 + (u8)p->unk7 * 100 + p->unk8;
        gCamera.unk0 |= 0x1000000;
        gCamera.unk64 |= 0x1000000;
        p->unk0 = 0xff;
        return NULL;
    }
    task = Task_Create(StageObj_Main, WaterLevelTrigger_Destructor, 0, 0x500, 2, sizeof(UnkStruct_0231ba3c));
    if (task == OS_GetArenaLo(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0231ba3c));
    MapObject_Init(w, (s32)p, b, c);
    w->base.unk58 |= 0x2100;
    w->base.unk5C |= 0x20;
    w->unk20E = (u8)p->unk6 * 10000 + (u8)p->unk7 * 100 + p->unk8;
    if (p->unk4 & 0x10)
        WaterLevelTrigger_State_ApplyOnce(w);
    else
        WaterLevelTrigger_Action_InitBySide(w);
    return w;
}
