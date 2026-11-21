#include <nitro/types.h>

typedef struct {
    void (*fn)(void);
    s32 mode;
    u8 pad8[4];
    s32 unkc;
    u8 pad10[0xa];
    u16 unk1a;
} UnkWork_02041924;

typedef struct {
    u8 pad0[8];
    void (*fn)(void);
} UnkTask_02041924;

extern u32 gSystemFlags;
extern u16 *gWirelessRecvBuffers;
extern UnkTask_02041924 *gTaskCurrent;

u32 WH_GetSystemState(void);
void WirelessMgr_State_Error(void);
void WirelessMgr_Main(void);
void WirelessMgr_State_BeginRestart(void);

void WirelessMgr_State_LobbyWaitStart(UnkWork_02041924 *w) {
    switch (WH_GetSystemState()) {
    case 8:
    case 9:
    case 10:
        w->unkc = 2;
        w->fn = WirelessMgr_State_Error;
        break;
    case 5:
        gSystemFlags |= 0x200;
        if (gWirelessRecvBuffers[1] & 1) {
            gTaskCurrent->fn = WirelessMgr_Main;
            w->fn = WirelessMgr_State_BeginRestart;
        }
        w->unk1a = 0;
        break;
    }
}
