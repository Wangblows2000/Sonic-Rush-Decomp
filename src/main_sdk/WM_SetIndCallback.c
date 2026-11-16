#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
u32 WMi_CheckInitialized(void);
u8 *WMi_GetSystemWork(void);
u32 WM_SetIndCallback(u32 v) {
    u32 e = OS_DisableInterrupts();
    u32 r = WMi_CheckInitialized();
    if (r != 0) { OS_RestoreInterrupts(e); return r; }
    *(u32 *)(WMi_GetSystemWork() + 0xbc) = v;
    OS_RestoreInterrupts(e);
    return 0;
}
