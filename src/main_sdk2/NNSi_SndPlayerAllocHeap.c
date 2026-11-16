#include <nitro/types.h>

typedef struct {
    u8 pad0[0xc];
    u8 list[0x18];
} UnkHeap_0201b8e8;

typedef struct {
    u8 pad0[8];
    void *seq;
    void *owner;
} UnkObj_0201b8e8;

typedef struct {
    u8 pad0[8];
    UnkObj_0201b8e8 *obj;
} UnkOwner_0201b8e8;

extern UnkHeap_0201b8e8 data_020838a4[];

void *NNS_FndGetNextListObject(void *list, void *obj);
void NNS_FndRemoveListObject(void *list, void *obj);
void NNS_SndHeapClear(void *seq);

void *NNSi_SndPlayerAllocHeap(s32 idx, UnkOwner_0201b8e8 *owner)
{
    UnkHeap_0201b8e8 *heap = &data_020838a4[idx];
    UnkObj_0201b8e8 *obj = NNS_FndGetNextListObject(heap->list, NULL);
    if (obj == NULL) {
        return NULL;
    }
    NNS_FndRemoveListObject(heap->list, obj);
    obj->owner = owner;
    owner->obj = obj;
    NNS_SndHeapClear(obj->seq);
    return obj->seq;
}
