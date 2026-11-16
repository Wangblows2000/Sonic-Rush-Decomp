#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
void *NNS_FndGetNextListObject(void *list, void *obj);
void NNS_FndRemoveListObject(void *list, void *obj);

typedef struct { u8 unk0[0x118]; s32 unk118; } UnkOwner_0201f988;
typedef struct { u32 unk0; u32 unk4; UnkOwner_0201f988 *owner; } UnkObj_0201f988;

UnkObj_0201f988 *func_0201f988(void *list) {
    u32 e = OS_DisableInterrupts();
    UnkObj_0201f988 *obj = NNS_FndGetNextListObject(list, NULL);
    if (obj != NULL) {
        NNS_FndRemoveListObject(list, obj);
        obj->owner->unk118--;
    }
    OS_RestoreInterrupts(e);
    return obj;
}
