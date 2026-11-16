#include <nitro/types.h>

typedef struct {
    u16 val;
    u16 flags;
} UnkStruct_022501a0;

typedef struct {
    void (*fn)(void);
    s32 mode;
    u8 pad8[4];
    s32 unkc;
    u8 pad10[4];
    u16 unk14;
    u8 pad16[2];
    u16 cnt;
    u16 unk1a;
} UnkWork_02041698;

extern UnkStruct_022501a0 data_022501a0;

u32 WH_GetSystemState(void);
s32 WFS_GetStatus(void);
void WH_SendData(void *p, u32 size, void (*fn)(void));
void WH_SetReceiver(void (*fn)(void));
void WirelessMgr_ChildSendCallback(void);
void WirelessMgr_State_SetupRecvBuffers(void);
void WirelessMgr_State_Error(void);

void WirelessMgr_State_ChildWaitReconnected(UnkWork_02041698 *w) {
    UnkStruct_022501a0 *p = &data_022501a0;

    switch (WH_GetSystemState()) {
    case 8:
    case 9:
    case 10:
        w->unkc = 2;
        w->fn = WirelessMgr_State_Error;
        break;
    case 5:
        if (!(p->flags & 2))
            return;
        w->cnt = 4;
        w->unk1a = 0;
        w->fn = WirelessMgr_State_SetupRecvBuffers;
        break;
    case 4:
        if (WFS_GetStatus() != 2)
            return;
        if (!(p->flags & 1))
            return;
        WH_SendData(p, 4, WirelessMgr_ChildSendCallback);
        WH_SetReceiver(NULL);
        w->cnt = 4;
        w->unk1a = 0;
        w->fn = WirelessMgr_State_SetupRecvBuffers;
        break;
    }
}
