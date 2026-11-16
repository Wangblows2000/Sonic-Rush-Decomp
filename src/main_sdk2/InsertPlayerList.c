#include <nitro/types.h>

typedef struct {
    void *handle;
    void *heap;
    u8 pad8[0x3d - 8];
    u8 prio;
} UnkSndPlayer_0201b840;

void *NNS_FndGetNextListObject(void *list, void *obj);
void NNS_FndInsertListObject(void *list, void *before, void *obj);

void InsertPlayerList(void *heap, UnkSndPlayer_0201b840 *p)
{
    UnkSndPlayer_0201b840 *it = NNS_FndGetNextListObject(heap, NULL);
    while (it != NULL) {
        if (p->prio < it->prio) {
            break;
        }
        it = NNS_FndGetNextListObject(heap, it);
    }
    NNS_FndInsertListObject(heap, it, p);
    p->heap = heap;
}
