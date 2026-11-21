#include <ov000_b/stageobj.h>

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0xE34 - 0x24];
} UnkStruct_0235f17c;

typedef struct {
    u8 pad0[8];
    s16 unk8;
} UnkStruct_0235f17c_Bright;

extern UnkStruct_0235f17c_Bright data_0208926c;
extern UnkStruct_0235f17c_Bright data_02087d44;

void ResultsScreen_State_Sync(void);
void ResultsScreen_Destructor(void);
BOOL ResultsScreen_Unk_ov000_0235cbd8(void);
u8 *ResultsScreen_GetLocalRecord(UnkStruct_0235f17c *w);
void OS_GetMacAddress(void *p);
void GXx_SetMasterBrightness_(vu16 *reg, s32 brightness);

#pragma thumb on

UnkStruct_0235f17c *ResultsScreen_CreateTask(s32 a, s32 b, s32 c) {
    UnkStruct_Task *task;
    UnkStruct_0235f17c *w;
    vu16 zero;
    u8 *p;
    s16 br;

    task = Task_Create(ResultsScreen_State_Sync, ResultsScreen_Destructor, 0, 0x1000, 0x1000, sizeof(UnkStruct_0235f17c));
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0235f17c));
    w->unk0 = 0;
    w->unk4 = 0;
    w->unk8 = 0;
    w->unk18 = a;
    w->unk1C = b;
    w->unk20 = c;
    w->unkC = 0;
    w->unk10 = ResultsScreen_Unk_ov000_0235cbd8();
    w->unk14 = data_0208926c.unk8;
    if (w->unk14 == 0)
        w->unk14 = -0x10;
    p = ResultsScreen_GetLocalRecord(w);
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;
    p[4] = 0;
    p[5] = 0;
    p[6] = 0;
    p[7] = 0;
    p[8] = 0;
    p[9] = 0;
    p[10] = 0;
    p[11] = 0;
    OS_GetMacAddress(p + 6);
    br = data_0208926c.unk8;
    if (br != -0x10 || br != 0x10) {
        data_0208926c.unk8 = -0x10;
        data_02087d44.unk8 = -0x10;
        GXx_SetMasterBrightness_((vu16 *)0x400006c, -0x10);
        GXx_SetMasterBrightness_((vu16 *)0x400106c, -0x10);
    }
    return w;
}

#pragma thumb reset
