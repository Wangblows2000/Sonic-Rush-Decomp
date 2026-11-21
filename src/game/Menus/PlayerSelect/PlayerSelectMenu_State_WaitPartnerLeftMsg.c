#include <game/Stage/stageobj_alt.h>

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

BOOL MsgBox_IsClosed(void);
void GXx_SetMasterBrightness_(vu16 *reg, s32 brightness);
void Task_DestroyByTag(s32 a);
void PlayerSelectMenu_ExitBack(s32 a, s32 b);

#pragma thumb on

void PlayerSelectMenu_State_WaitPartnerLeftMsg(void) {
    s32 a;
    s32 b;
    UnkStruct_02358710 *w = gTaskCurrent->work;

    data_02087d44.unk8 = 0;
    data_0208926c.unk8 = data_02087d44.unk8;
    w->unk0++;
    if (MsgBox_IsClosed()) {
        data_02087d44.unk8 = -0x10;
        data_0208926c.unk8 = data_02087d44.unk8;
        GXx_SetMasterBrightness_((vu16 *)0x400006c, data_0208926c.unk8);
        GXx_SetMasterBrightness_((vu16 *)0x400106c, data_02087d44.unk8);
        a = w->unkC;
        b = w->unk14;
        Task_DestroyByTag(0x1000);
        PlayerSelectMenu_ExitBack(a, b);
    }
}

#pragma thumb reset
