#include <nitro/types.h>

typedef struct {
    u8 pad0[0x3d];
    u8 prio;
} UnkSndPlayer_0201b728;

extern u8 data_0208344c[];
extern u8 data_02083458[];

void *NNS_FndGetNextListObject(void *list, void *obj);
void NNS_FndRemoveListObject(void *list, void *obj);
void ForceStopSeq(UnkSndPlayer_0201b728 *p);
void InsertPrioList(UnkSndPlayer_0201b728 *p);

UnkSndPlayer_0201b728 *AllocSeqPlayer(s32 prio)
{
    UnkSndPlayer_0201b728 *p = NNS_FndGetNextListObject(data_0208344c, NULL);
    if (p == NULL) {
        p = NNS_FndGetNextListObject(data_02083458, NULL);
        if (prio < p->prio) {
            return NULL;
        }
        ForceStopSeq(p);
    }
    NNS_FndRemoveListObject(data_0208344c, p);
    p->prio = prio;
    InsertPrioList(p);
    return p;
}
