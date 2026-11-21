#include <nitro/types.h>
#include <NitroSDK/wm.h>

s32 WM_SetPortCallback(u16 port, void *cb, void *arg)
{
    u32 e = OS_DisableInterrupts();
    s32 r = WMi_CheckInitialized();
    u8 *work;
    if (r != 0) {
        OS_RestoreInterrupts(e);
        return r;
    }
    work = (u8 *)WMi_GetSystemWork();
    ((void **)(work + 0xc0))[port] = cb;
    ((void **)(work + 0x100))[port] = arg;
    OS_RestoreInterrupts(e);
    return 0;
}
