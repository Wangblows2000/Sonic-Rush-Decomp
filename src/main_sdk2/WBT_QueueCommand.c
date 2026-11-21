#include <nitro/types.h>

extern u32 data_02080600;
extern u8 data_020806b0[];

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
u32 *WBT_GetQueuedCommand(void);
void MIi_CpuCopy32(const void *src, void *dst, u32 size);

BOOL WBT_QueueCommand(const void *param)
{
    u32 e = OS_DisableInterrupts();
    u32 *c;
    if (data_02080600 != 1) {
        OS_RestoreInterrupts(e);
        return FALSE;
    }
    c = WBT_GetQueuedCommand();
    if (c == NULL || *c != 0) {
        OS_RestoreInterrupts(e);
        return FALSE;
    }
    MIi_CpuCopy32(param, data_020806b0, 0x9c);
    OS_RestoreInterrupts(e);
    return TRUE;
}
