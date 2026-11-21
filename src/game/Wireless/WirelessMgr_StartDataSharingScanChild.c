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
} UnkWork_0204361c;

typedef struct {
    u8 pad0[0x10];
    UnkWork_0204361c *work;
} UnkTask_0204361c;

extern UnkTask_0204361c *gWirelessMgrTask;
extern u8 gWirelessSsidData[];
extern u32 gSystemFlags;
extern void (*gWirelessErrorCallback)(void);

UnkTask_0204361c *Task_Create(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
void WirelessMgr_Main(void);
void WirelessMgr_Destructor(void);
void MIi_CpuClear16(u16 v, void *dst, u32 size);
void WirelessMgr_State_BeginScan(void);
void WirelessMgr_MainLobby(void);
void WH_SetMaxChildCount(u16 a);
void WH_SetPacketSize(u16 a);
void Wireless_SetupRecvBuffers(u16 a);
void WH_Initialize(void);
void WH_SetGgid(s32 a);
void MI_CpuCopy8(const void *src, void *dst, u32 size);
void WirelessMgr_OnConnectionLost(void);

void WirelessMgr_StartDataSharingScanChild(u8 a, u16 n, u16 b, void *data, u16 len) {
    UnkWork_0204361c *w;

    gWirelessMgrTask = Task_Create(WirelessMgr_MainLobby, WirelessMgr_Destructor, 0, 1, 0xfffe, sizeof(UnkWork_0204361c));
    w = gWirelessMgrTask->work;
    {
        vu16 zero = 0;
        MIi_CpuClear16(zero, w, sizeof(UnkWork_0204361c));
    }
    w->fn = WirelessMgr_State_BeginScan;
    w->unk4 = 1;
    w->unk8 = 4;
    w->unk16 = a;
    w->unk192a = 0xffff;
    w->unk1aee = b;
    if (w->unk1aee < 4)
        w->unk1aee = 4;
    WH_SetMaxChildCount(n);
    WH_SetPacketSize(4);
    Wireless_SetupRecvBuffers(4);
    WH_Initialize();
    WH_SetGgid(0x51);
    if (data != NULL && len != 0)
        MI_CpuCopy8(data, gWirelessSsidData, len);
    gSystemFlags &= ~0x100;
    gSystemFlags |= 0x200;
    gWirelessErrorCallback = WirelessMgr_OnConnectionLost;
}
