#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; } UnkStruct_0207ad9c;
extern void *data_022c4638;
extern void *data_022c4634;
extern u32 gSystemFlags;
extern UnkStruct_0207ad9c gWirelessSession;

void Task_Destroy(void *t);
void WirelessMgr_StopDataSharing(s32 a);
void WirelessMgr_StopWfs(s32 a);
void WirelessMgr_StopDownloadPlay(void);

#pragma thumb on
void func_02051838(void) {
    if (data_022c4638 != NULL) {
        Task_Destroy(data_022c4638);
        data_022c4638 = NULL;
    }
    if (data_022c4634 != NULL) {
        Task_Destroy(data_022c4634);
        data_022c4634 = NULL;
    }
    gSystemFlags &= ~0x200;
    switch (gWirelessSession.unk0) {
    case 0:
        WirelessMgr_StopDataSharing(0);
        break;
    case 1:
        WirelessMgr_StopWfs(0);
        break;
    case 2:
        WirelessMgr_StopDownloadPlay();
        break;
    }
}
#pragma thumb reset
