#include <nitro/types.h>
#include <main_sdk2/sndp.h>

extern UnkSndPlayerHeap data_020838a4[];

void NNS_SndHandleReleaseSeq(UnkSndHandle *handle);
void ForceStopSeq(UnkSndPlayer *p);
UnkSndPlayer *AllocSeqPlayer(s32 prio);
void InsertPlayerList(UnkSndPlayerHeap *heap, UnkSndPlayer *p);

UnkSndPlayer *NNSi_SndPlayerAllocSeqPlayer(UnkSndHandle *handle, s32 playerNo, s32 prio)
{
    UnkSndPlayerHeap *heap = &data_020838a4[playerNo];
    UnkSndPlayer *p;

    if (handle->player != NULL) {
        NNS_SndHandleReleaseSeq(handle);
    }
    if (heap->count >= heap->max) {
        p = NNS_FndGetNextListObject(heap, NULL);
        if (p == NULL) {
            return NULL;
        }
        if (prio < p->prio) {
            return NULL;
        }
        ForceStopSeq(p);
    }
    p = AllocSeqPlayer(prio);
    if (p == NULL) {
        return NULL;
    }
    InsertPlayerList(heap, p);
    p->handle = handle;
    handle->player = p;
    return p;
}
