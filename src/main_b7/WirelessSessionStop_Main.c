#include <nitro/types.h>

typedef struct {
    u32 unk0;
    u32 unk4;
    u32 unk8;
} UnkWork_02050430;

typedef struct {
    u32 unk0;
    u32 unk4;
    void (*fn)(void);
    u32 unkc;
    UnkWork_02050430 *work;
} UnkTask_02050430;

extern UnkTask_02050430 *gWirelessLobbyTask;
extern UnkTask_02050430 *data_022c4634;
extern UnkTask_02050430 *gTaskCurrent;
extern u32 gSystemFlags;
extern u16 gWirelessSession;
void WirelessMgr_StopDataSharing(s32 a);
void WirelessMgr_StopWfs(s32 a);
void WirelessMgr_StopDownloadPlay(void);
void Task_Destroy(UnkTask_02050430 *t);

#pragma thumb on
void WirelessSessionStop_Main(void) {
    u32 a;
    u32 b;
    if (gWirelessLobbyTask == NULL) {
        u32 v;
        u32 x;
        u32 y;
        UnkWork_02050430 *w;
        w = gTaskCurrent->work;
        v = w->unk0;
        x = v & 0xff;
        y = v >> 8;
        if (x > 1)
            x--;
        if (y > 1)
            y--;
        a = x;
        b = y;
        if (x == 1)
            x = 0;
        if (y == 1)
            y = 0;
        w->unk0 = x | (y << 8);
    } else {
        UnkWork_02050430 *w = gWirelessLobbyTask->work;
        u32 v = w->unk8;
        u32 x = v & 0xff;
        u32 y = v >> 8;
        if (x > 1)
            x--;
        if (y > 1)
            y--;
        a = x;
        b = y;
        if (x == 1)
            x = 0;
        if (y == 1)
            y = 0;
        w->unk8 = x | (y << 8);
    }
    if (a == 1) {
        gSystemFlags &= ~0x200;
        switch (gWirelessSession) {
        case 0:
            WirelessMgr_StopDataSharing(0);
            break;
        case 1:
            WirelessMgr_StopWfs(0);
            break;
        case 2:
            WirelessMgr_StopDownloadPlay();
            break;
        default:
            return;
        }
        a = 0;
        gWirelessSession = 4;
    }
    if (a == 0 && b <= 1) {
        Task_Destroy(gTaskCurrent);
        data_022c4634 = NULL;
    }
}
#pragma thumb reset
