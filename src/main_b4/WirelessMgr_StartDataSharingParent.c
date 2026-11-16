#include <nitro/types.h>

typedef struct {
    void (*fn)(void);
    s32 unk4;
    s32 unk8;
    u8 padc[4];
    u16 unk10;
    u16 unk12;
    u16 mask;
    u8 unk16;
    u8 pad17[5];
    void *unk1c;
    u32 unk20;
    u8 pad24[0x192a - 0x24];
    u16 unk192a;
    u8 pad192c[0x1aee - 0x192c];
    u16 unk1aee;
    u8 pad1af0[0x1cf4 - 0x1af0];
} UnkWork_02043758;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02043758 *work;
} UnkTask_02043758;

extern UnkTask_02043758 *gWirelessMgrTask;
extern u8 gWirelessUserGameInfo[];
extern u32 gSystemFlags;
extern void (*gWirelessErrorCallback)(void);

UnkTask_02043758 *Task_Create(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
void WirelessMgr_Main(void);
void WirelessMgr_Destructor(void);
void MIi_CpuClear16(u16 v, void *dst, u32 size);
void WirelessMgr_State_ParentConnect(void);
void WH_SetMaxChildCount(u16 a);
void WH_SetPacketSize(u16 a);
void Wireless_SetupRecvBuffers(u16 a);
void WH_Initialize(void);
void WH_SetGgid(s32 a);
void WirelessMgr_JudgeAcceptChild(void);
void WH_SetJudgeAcceptFunc(void (*f)(void));
void MI_CpuCopy8(const void *src, void *dst, u32 size);
void WirelessMgr_OnConnectionLost(void);

void WirelessMgr_StartDataSharingParent(u16 *info, void *a, u16 b, void *data, u16 len) {
    UnkWork_02043758 *w;
    u16 i;

    gWirelessMgrTask = Task_Create(WirelessMgr_Main, WirelessMgr_Destructor, 0, 1, 0xfffe, sizeof(UnkWork_02043758));
    w = gWirelessMgrTask->work;
    {
        vu16 zero = 0;
        MIi_CpuClear16(zero, w, sizeof(UnkWork_02043758));
    }
    w->fn = WirelessMgr_State_ParentConnect;
    w->unk4 = 1;
    w->unk8 = 2;
    w->unk10 = info[0];
    w->unk12 = info[2];
    w->unk16 = 0x19;
    w->unk192a = 0xffff;
    w->unk1aee = b;
    w->unk1c = a;
    i = 0;
    if (w->unk1aee < 4)
        w->unk1aee = 4;
    for (; i < info[1]; i++)
        w->mask = (w->mask << 1) | 1;
    WH_SetMaxChildCount(((u16 *)info)[1] - 1);
    WH_SetPacketSize(w->unk1aee);
    Wireless_SetupRecvBuffers(w->unk1aee);
    WH_Initialize();
    WH_SetGgid(0x51);
    WH_SetJudgeAcceptFunc(WirelessMgr_JudgeAcceptChild);
    if (data != NULL && len != 0) {
        w->unk20 = len;
        MI_CpuCopy8(data, gWirelessUserGameInfo, len);
    }
    gSystemFlags &= ~0x100;
    gSystemFlags |= 0x200;
    gWirelessErrorCallback = WirelessMgr_OnConnectionLost;
}
