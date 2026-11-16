#ifndef MAIN_SDK2_WM_H
#define MAIN_SDK2_WM_H
#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
s32 WMi_CheckStateEx(u32 num, ...);
s32 WMi_CheckIdle(void);
s32 WMi_CheckInitialized(void);
struct UnkWmWork_;
void *WMi_GetSystemWork(void);
void WMi_SetCallbackTable(u32 apiid, void *cb);
s32 WMi_SendCommand(u32 apiid, u32 paramNum, ...);
#endif

#ifndef MAIN_SDK2_WM_WORK
#define MAIN_SDK2_WM_WORK
typedef struct {
    u32 unk0;
    u8 *unk4;
    u32 unk8;
    u16 *unkC;
    u32 unk10;
} UnkWmWork;
#endif
