#include <nitro/types.h>

typedef struct {
    void (*fn)(void);
    u8 pad4[8];
    s32 unkc;
} UnkWork_02041300;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02041300 *work;
} UnkTask_02041300;

extern u32 gSystemFlags;
extern UnkTask_02041300 *gWirelessMgrTask;

void WH_Reset(void);
s32 WH_GetSystemState(void);
void WH_End(void);
void WirelessMgr_State_Error(void);

void WirelessMgr_OnConnectionLost(void) {
    BOOL loop = TRUE;

    gSystemFlags &= 0x100;
    WH_Reset();
    do {
        switch (WH_GetSystemState()) {
        case 1:
            WH_End();
        case 9:
        case 10:
            loop = FALSE;
            break;
        }
    } while (loop);
    if (gWirelessMgrTask != NULL) {
        UnkWork_02041300 *w = gWirelessMgrTask->work;
        w->unkc = 2;
        w->fn = WirelessMgr_State_Error;
    }
}
