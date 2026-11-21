#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
void *NNS_FndGetNextListObject(void *list, void *obj);
void NNS_FndRemoveListObject(void *list, void *obj);
void FreeCommandBuffer(void *obj);

typedef struct { u32 unk0; u32 unk4; void *owner; } UnkObj_0201f9dc;

void RemoveCommandByPlayer(void *list, void *owner) {
    u32 e = OS_DisableInterrupts();
    UnkObj_0201f9dc *obj = NNS_FndGetNextListObject(list, NULL);
    while (obj != NULL) {
        UnkObj_0201f9dc *next = NNS_FndGetNextListObject(list, obj);
        if (obj->owner == owner) {
            NNS_FndRemoveListObject(list, obj);
            FreeCommandBuffer(obj);
        }
        obj = next;
    }
    OS_RestoreInterrupts(e);
}
