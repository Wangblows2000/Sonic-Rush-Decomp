#include <nitro/types.h>

typedef struct {
    void (*fn)(void);
    s32 mode;
    u8 pad8[8];
    u16 unk10;
    u16 unk12;
    u8 pad14[6];
    u16 unk1a;
    u8 pad1c[0x1aee - 0x1c];
    u16 unk1aee;
    u16 unk1af0;
    u16 unk1af2;
} UnkWork_02041fac;

extern u32 gSystemFlags;
extern u8 gWirelessUserGameInfo[];

u32 WH_GetSystemState(void);
void WH_SetUserGameInfo(void *p, u32 size);
void WH_SetPacketSize(u16 a);
void Wireless_SetupRecvBuffers(u16 a);
void WH_SetMaxParentChildSize(u16 a, u16 b);
void WH_ParentConnect(s32 state, u16 a, u16 b);
void WirelessMgr_State_ParentWaitChildrenReconnect(void);

void WirelessMgr_State_ParentReconnect(UnkWork_02041fac *w) {
    s32 state;

    if (WH_GetSystemState() != 1)
        return;
    WH_SetUserGameInfo(gWirelessUserGameInfo, 0x70);
    w->unk12 |= 0x8000;
    switch (w->mode) {
    case 1:
        state = 4;
        gSystemFlags |= 0x100;
        gSystemFlags &= ~0x200;
        WH_SetPacketSize(w->unk1aee);
        Wireless_SetupRecvBuffers(w->unk1aee);
        break;
    case 2:
        state = 6;
        WH_SetMaxParentChildSize(w->unk1af0, w->unk1af2);
        break;
    }
    WH_ParentConnect(state, w->unk12, w->unk10);
    w->unk1a = 0;
    w->fn = WirelessMgr_State_ParentWaitChildrenReconnect;
}
