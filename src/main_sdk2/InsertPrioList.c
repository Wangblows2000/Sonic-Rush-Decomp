#include <nitro/types.h>

typedef struct {
    u8 pad0[0x3d];
    u8 prio;
} UnkSndPlayer_0201b7e0;

extern u8 data_02083458[];

void *NNS_FndGetNextListObject(void *list, void *obj);
void NNS_FndInsertListObject(void *list, void *before, void *obj);

void InsertPrioList(UnkSndPlayer_0201b7e0 *p)
{
    UnkSndPlayer_0201b7e0 *it = NNS_FndGetNextListObject(data_02083458, NULL);
    while (it != NULL) {
        if (p->prio < it->prio) {
            break;
        }
        it = NNS_FndGetNextListObject(data_02083458, it);
    }
    NNS_FndInsertListObject(data_02083458, it, p);
}
