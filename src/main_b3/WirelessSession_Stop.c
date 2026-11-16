#include <nitro/types.h>

typedef struct { s32 unk0; s32 unk4; } UnkWork_020516b4;
typedef struct { u8 pad0[0x10]; UnkWork_020516b4 *work; } UnkTask_020516b4;
typedef struct { u16 unk0; u16 unk2; } UnkStruct_0207ad9c;
extern UnkTask_020516b4 *data_022c4638;
extern UnkTask_020516b4 *gTaskCurrent;
extern UnkStruct_0207ad9c gWirelessSession;
extern u32 gSystemFlags;
extern u8 data_022c44f8[];
extern u8 data_022c4500[];

void Wireless_EnableErrorCallback(s32 a);
BOOL WirelessMgr_GetStatus(void);
void WirelessMgr_GetHostInfo_02043204(void *p);
void WirelessMgr_StopDataSharing(s32 a);
void WirelessMgr_GetHostInfo(void *p);
void WirelessMgr_StopWfs(s32 a);
void WirelessMgr_GetHostInfo_02042a70(void *p);
void WirelessMgr_StopDownloadPlay(void);
void Task_Destroy(UnkTask_020516b4 *t);
void func_020508a8(s32 a, s32 b);
void WirelessMgr_GetJoinInfo_0204319c(void *p);
void WirelessMgr_GetJoinInfo(void *p);
void func_02042a20(void *p, s32 a);

#pragma thumb on
void WirelessSession_Stop(void) {
    if (data_022c4638 != NULL) {
        UnkWork_020516b4 *w = data_022c4638->work;
        gWirelessSession.unk0 = w->unk0;
        gWirelessSession.unk2 = w->unk4;
    }
    Wireless_EnableErrorCallback(0);
    switch (gWirelessSession.unk2) {
    case 0:
    case 1:
        switch (gWirelessSession.unk0) {
        case 0:
            gSystemFlags |= 0x200;
            if (WirelessMgr_GetStatus())
                WirelessMgr_GetHostInfo_02043204(data_022c44f8);
            WirelessMgr_StopDataSharing(1);
            break;
        case 1:
            if (WirelessMgr_GetStatus())
                WirelessMgr_GetHostInfo(data_022c44f8);
            WirelessMgr_StopWfs(1);
            break;
        case 2:
            if (WirelessMgr_GetStatus())
                WirelessMgr_GetHostInfo_02042a70(data_022c44f8);
            WirelessMgr_StopDownloadPlay();
            gWirelessSession.unk0 = 4;
            Task_Destroy(gTaskCurrent);
            data_022c4638 = NULL;
            return;
        default:
            return;
        }
        func_020508a8(0x18, 0x20);
        break;
    case 2:
    case 3:
        switch (gWirelessSession.unk0) {
        case 0:
            gSystemFlags |= 0x200;
            if (WirelessMgr_GetStatus())
                WirelessMgr_GetJoinInfo_0204319c(data_022c4500);
            WirelessMgr_StopDataSharing(1);
            break;
        case 1:
            if (WirelessMgr_GetStatus())
                WirelessMgr_GetJoinInfo(data_022c4500);
            WirelessMgr_StopWfs(1);
            break;
        case 2:
            if (WirelessMgr_GetStatus())
                func_02042a20(data_022c4500, 1);
            WirelessMgr_StopDownloadPlay();
            gWirelessSession.unk0 = 4;
            Task_Destroy(gTaskCurrent);
            data_022c4638 = NULL;
            return;
        default:
            return;
        }
        func_020508a8(0, 0x20);
        break;
    }
}
#pragma thumb reset
