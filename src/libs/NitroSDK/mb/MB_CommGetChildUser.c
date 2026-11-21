#include <nitro/types.h>
#include <NitroSDK/wh.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
BOOL IsChildAidValid(u16 aid);
void MI_CpuCopy8(const void *src, void *dest, u32 size);

UnkInitParam_wh *MB_CommGetChildUser(u16 aid)
{
    u32 e = OS_DisableInterrupts();
    if (data_02080f1c != NULL && IsChildAidValid(aid)) {
        MI_CpuCopy8(&data_02080f1c->childInit[aid - 1], &data_02080f1c->unk1772, sizeof(UnkInitParam_wh));
        OS_RestoreInterrupts(e);
        return &data_02080f1c->unk1772;
    }
    OS_RestoreInterrupts(e);
    return NULL;
}
