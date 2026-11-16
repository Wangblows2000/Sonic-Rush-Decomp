#include <nitro/types.h>
#include <nitro/fs_overlay.h>

extern s32 data_0224ff40;

void SysSound_Release(void);
void NNS_SndStopSoundAll(void);
void Sound_ResetHeap(void);
void func_0204331c(s32 mode);
void func_02054a04(void);
void Sound_Init(void);
void Overlay_Request(u32 overlayId, void (*fn)(void));
void func_02054d70(void);

#pragma thumb on
void func_02054dbc(void) {
    SysSound_Release();
    NNS_SndStopSoundAll();
    Sound_ResetHeap();
    data_0224ff40 = 0;
    func_0204331c(0);
    func_02054a04();
    Sound_Init();
    Overlay_Request(FS_OVERLAY_ID(ov001), func_02054d70);
}
#pragma thumb reset
