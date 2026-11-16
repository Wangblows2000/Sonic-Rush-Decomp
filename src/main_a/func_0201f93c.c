#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
void *NNS_FndGetNextListObject(void *list, void *obj);
void NNS_FndRemoveListObject(void *list, void *obj);
extern u8 data_02083f50[];

void *func_0201f93c(void) {
    u32 e = OS_DisableInterrupts();
    void *obj = NNS_FndGetNextListObject(data_02083f50, NULL);
    if (obj != NULL) {
        NNS_FndRemoveListObject(data_02083f50, obj);
    }
    OS_RestoreInterrupts(e);
    return obj;
}
