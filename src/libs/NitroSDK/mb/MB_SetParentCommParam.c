#include <nitro/types.h>
#include <NitroSDK/wh.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
BOOL MBi_IsCommSizeValid(u32 parentSize, u32 childSize, u32 maxEntry);

BOOL MB_SetParentCommParam(u16 parentSize, u16 maxEntry)
{
    u32 e = OS_DisableInterrupts();
    if (data_02080f18->unk50d != 0) {
        OS_RestoreInterrupts(e);
        return FALSE;
    }
    if (!MBi_IsCommSizeValid(parentSize, 8, maxEntry)) {
        OS_RestoreInterrupts(e);
        return FALSE;
    }
    data_02080f18->parentParam.maxEntry = maxEntry;
    data_02080f18->unk500 = parentSize;
    data_02080f18->unk502 = 8;
    OS_RestoreInterrupts(e);
    return TRUE;
}
