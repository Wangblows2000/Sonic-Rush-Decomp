#include <nitro/types.h>
#include <nitro/fs_overlay.h>

extern s32 gWirelessErrorCallback;

void SysSound_Release(void);
void NNS_SndStopSoundAll(void);
void Sound_ResetHeap(void);
void WirelessMgr_StopDataSharing(s32 mode);
void WirelessError_ResetSystem(void);
void Sound_Init(void);
void Overlay_Request(u32 overlayId, void (*fn)(void));
void WirelessError_Start(void);

#pragma thumb on
void Wireless_OnError(void) {
    SysSound_Release();
    NNS_SndStopSoundAll();
    Sound_ResetHeap();
    gWirelessErrorCallback = 0;
    WirelessMgr_StopDataSharing(0);
    WirelessError_ResetSystem();
    Sound_Init();
    Overlay_Request(FS_OVERLAY_ID(ov001), WirelessError_Start);
}
#pragma thumb reset
