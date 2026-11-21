#include <nitro/types.h>

typedef struct {
    u16 val;
    u16 flags;
} UnkStruct_0224ffa0;

typedef struct {
    void (*fn)(void);
    s32 mode;
    u8 pad8[0xc];
    u16 unk14;
    u8 pad16[2];
    u16 cnt;
    u16 unk1a;
    u32 unk1c;
} UnkWork_02041e90;

extern UnkStruct_0224ffa0 gWirelessSendBuffer;

u16 WH_GetBitmap(void);
u32 WFS_GetCurrentBitmap(void);
u32 Math_CountBits16(u16 v);
s32 WFS_GetStatus(void);
void WH_SendData(void *p, u32 size, void (*fn)(void));
void WH_SetReceiver(void (*fn)(void));
void WFS_EnableSync(u16 v);
void DC_StoreRange(void *p, u32 size);
void WirelessMgr_ParentReceiver(void);
void WirelessMgr_ParentSendCallback(void);
void WirelessMgr_State_SetupRecvBuffers(void);
void WirelessMgr_State_ParentWaitChildAckReconnect(void);

void WirelessMgr_State_ParentWaitChildrenReconnect(UnkWork_02041e90 *w) {
    UnkStruct_0224ffa0 *p;
    u16 v;

    switch (w->mode) {
    case 1:
        v = WH_GetBitmap();
        break;
    case 2:
        v = WFS_GetCurrentBitmap();
        break;
    }
    if (Math_CountBits16(w->unk14) != Math_CountBits16(v))
        return;
    p = &gWirelessSendBuffer;
    switch (w->mode) {
    case 1:
        p->val = v;
        p->flags |= 2;
        w->cnt = 5;
        w->unk1a = 0;
        w->fn = WirelessMgr_State_SetupRecvBuffers;
        break;
    case 2:
        if (WFS_GetStatus() != 2)
            return;
        WH_SetReceiver(WirelessMgr_ParentReceiver);
        p->val = v;
        p->flags = 1;
        DC_StoreRange(p, 4);
        WH_SendData(p, 4, WirelessMgr_ParentSendCallback);
        WFS_EnableSync(v);
        w->unk1a = 0;
        w->fn = WirelessMgr_State_ParentWaitChildAckReconnect;
        break;
    }
}
