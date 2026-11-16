#include <ov000_b/stageobj.h>

typedef struct {
    s32 unk0;
    u8 pad4[8];
    s32 unkC;
    u8 pad10[4];
    s32 unk14;
} UnkStruct_02358710;

typedef struct {
    u8 pad0[8];
    s16 unk8;
} UnkStruct_02358710_Bright;

extern UnkStruct_Task *gTaskCurrent;
extern UnkStruct_02358710_Bright data_0208926c;
extern UnkStruct_02358710_Bright data_02087d44;

BOOL func_02050018(void);
void GXx_SetMasterBrightness_(vu16 *reg, s32 brightness);
void Task_DestroyByTag(s32 a);
void func_ov000_02355dac(s32 a, s32 b);

#pragma thumb on

void func_ov000_02358710(void) {
    s32 a;
    s32 b;
    UnkStruct_02358710 *w = gTaskCurrent->work;

    data_02087d44.unk8 = 0;
    data_0208926c.unk8 = data_02087d44.unk8;
    w->unk0++;
    if (func_02050018()) {
        data_02087d44.unk8 = -0x10;
        data_0208926c.unk8 = data_02087d44.unk8;
        GXx_SetMasterBrightness_((vu16 *)0x400006c, data_0208926c.unk8);
        GXx_SetMasterBrightness_((vu16 *)0x400106c, data_02087d44.unk8);
        a = w->unkC;
        b = w->unk14;
        Task_DestroyByTag(0x1000);
        func_ov000_02355dac(a, b);
    }
}

#pragma thumb reset
