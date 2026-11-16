#include <nitro/types.h>

typedef struct { u8 pad0[8]; s32 unk8; s32 unkC; } UnkWork_02050344;
typedef struct { u8 pad0[0x10]; UnkWork_02050344 *work; } UnkTask_02050344;
extern UnkTask_02050344 *gTaskCurrent;
extern s32 data_022c463c;
extern void *data_022c4638;

void WirelessSession_ForceStop(void);
s32 WirelessMgr_GetStatus(void);
s32 WirelessLobby_GetChildCount(void);
void WirelessMgr_RequestStart(void);
void WirelessSession_Stop(void);

#pragma thumb on
void WirelessLobbyPreview_Main(void) {
    UnkWork_02050344 *w = gTaskCurrent->work;

    if (w->unkC != 0) {
        WirelessSession_ForceStop();
        data_022c463c = 0;
        return;
    }
    switch (WirelessMgr_GetStatus()) {
    case 2:
        if (w->unk8 != 0) {
            w->unk8 = 0;
            if (WirelessLobby_GetChildCount() > 0) {
                WirelessMgr_RequestStart();
                return;
            }
            WirelessSession_ForceStop();
            data_022c463c = 1;
            return;
        }
        break;
    case 6:
        WirelessSession_Stop();
        WirelessSession_ForceStop();
        data_022c463c = 0;
        data_022c4638 = NULL;
        return;
    case 7:
        WirelessSession_ForceStop();
        data_022c463c = 1;
        break;
    case 0:
    case 1:
    case 3:
    case 4:
    case 5:
        break;
    }
}
#pragma thumb reset
