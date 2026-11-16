#include <nitro/types.h>

typedef struct {
    u8 data[0x24];
} UnkHeap_0201bf40;

extern UnkHeap_0201bf40 data_020838a4[];

void *NNS_FndGetNextListObject(void *list, void *obj);
void func_0201b944(void *p, s32 frames);

void func_0201bf40(s32 idx, s32 frames)
{
    void *next;
    void *p;
    UnkHeap_0201bf40 *heap = &data_020838a4[idx];
    p = NNS_FndGetNextListObject(heap, NULL);
    if (p == NULL) {
        return;
    }
    do {
        next = NNS_FndGetNextListObject(heap, p);
        func_0201b944(p, frames);
        p = next;
    } while (next != NULL);
}
