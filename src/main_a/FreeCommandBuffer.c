#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
void NNS_FndAppendListObject(void *list, void *obj);
extern u8 data_02083f50[];

void FreeCommandBuffer(void *obj) {
    u32 e = OS_DisableInterrupts();
    NNS_FndAppendListObject(data_02083f50, obj);
    OS_RestoreInterrupts(e);
}
