#include <nitro/types.h>

typedef struct {
    void *handle;
    void *heap;
    u8 pad8[0x3d - 8];
    u8 prio;
} UnkSndPlayer_0201b5d8;

extern u8 data_02083458[];

void NNS_FndRemoveListObject(void *list, void *obj);
void InsertPlayerList(void *heap, UnkSndPlayer_0201b5d8 *p);
void InsertPrioList(UnkSndPlayer_0201b5d8 *p);

void SetPlayerPriority(UnkSndPlayer_0201b5d8 *p, u8 prio)
{
    void *heap = p->heap;
    if (heap != NULL) {
        NNS_FndRemoveListObject(heap, p);
        p->heap = NULL;
    }
    NNS_FndRemoveListObject(data_02083458, p);
    p->prio = prio;
    if (heap != NULL) {
        InsertPlayerList(heap, p);
    }
    InsertPrioList(p);
}
