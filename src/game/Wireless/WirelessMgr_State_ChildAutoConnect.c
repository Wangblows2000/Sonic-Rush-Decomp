#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    u8 unk4[0x32];
    u16 unk36;
    u8 pad38[200 - 0x38];
} UnkEntry_020415a8;

typedef struct {
    void (*fn)(void);
    s32 mode;
    u8 pad8[0x10];
    u16 cnt;
    u16 unk1a;
    u8 pad1c[0xc];
    UnkEntry_020415a8 entries[32];
    u8 pad1928[2];
    u16 idx;
    u8 pad192c[0x1aee - 0x192c];
    u16 unk1aee;
    u16 unk1af0;
    u16 unk1af2;
} UnkWork_020415a8;

extern u32 gSystemFlags;

u32 WH_GetSystemState(void);
void WH_SetPacketSize(u16 a);
void Wireless_SetupRecvBuffers(u16 a);
void WH_SetMaxParentChildSize(u16 a, u16 b);
void WH_SetReceiver(void (*fn)(void));
void WH_ChildConnectAuto(void (*fn)(void), s32 a, void *p, u16 b);
void WirelessMgr_ChildReceiver(void);
void WirelessMgr_OnAutoConnectFoundParent(void);
void WirelessMgr_State_ChildWaitConnected(void);

void WirelessMgr_State_ChildAutoConnect(UnkWork_020415a8 *w) {
    UnkEntry_020415a8 *e;
    s32 state;

    if (WH_GetSystemState() != 1)
        return;
    w->unk1a = 0;
    if (w->cnt >= 10) {
        e = &w->entries[w->idx];
        switch (w->mode) {
        case 1:
            state = 5;
            gSystemFlags |= 0x100;
            gSystemFlags &= ~0x200;
            WH_SetPacketSize(w->unk1aee);
            Wireless_SetupRecvBuffers(w->unk1aee);
            break;
        case 2:
            state = 7;
            WH_SetMaxParentChildSize(w->unk1af0, w->unk1af2);
            WH_SetReceiver(WirelessMgr_ChildReceiver);
            break;
        }
        WH_ChildConnectAuto(WirelessMgr_OnAutoConnectFoundParent, state, e->unk4, e->unk36);
        w->fn = WirelessMgr_State_ChildWaitConnected;
    } else {
        w->cnt++;
    }
}
