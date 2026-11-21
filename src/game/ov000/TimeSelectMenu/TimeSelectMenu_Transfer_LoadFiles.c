#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    void (*unk8)(void);
} UnkStruct_02361378_Task;

extern UnkStruct_02361378_Task *gTaskCurrent;

s32 StageLoader_LoadZoneFiles(void);
void Wireless_OnError(void);
u32 WH_GetBitmap(void);
void WirelessSession_Stop(void);
void TimeSelectMenu_Transfer_RestartDataSharing(void);

#pragma thumb on

void TimeSelectMenu_Transfer_LoadFiles(void) {
    s32 s = StageLoader_LoadZoneFiles();

    if (s == 2) {
        Wireless_OnError();
        return;
    }
    if (WH_GetBitmap() & 0) {
        Wireless_OnError();
        return;
    }
    if (s == 1) {
        WirelessSession_Stop();
        gTaskCurrent->unk8 = TimeSelectMenu_Transfer_RestartDataSharing;
    }
}

#pragma thumb reset
