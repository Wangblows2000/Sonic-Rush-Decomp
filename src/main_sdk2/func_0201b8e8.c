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
void func_0202061c(void *list, void *obj);
void func_0201da54(void *seq);

void *func_0201b8e8(s32 idx, UnkOwner_0201b8e8 *owner)
{
    UnkHeap_0201b8e8 *heap = &data_020838a4[idx];
    UnkObj_0201b8e8 *obj = NNS_FndGetNextListObject(heap->list, NULL);
    if (obj == NULL) {
        return NULL;
    }
    func_0202061c(heap->list, obj);
    obj->owner = owner;
    owner->obj = obj;
    func_0201da54(obj->seq);
    return obj->seq;
}
