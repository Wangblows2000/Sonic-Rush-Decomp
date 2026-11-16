#include <nitro/types.h>

typedef struct {
    u8 pad0[0x1c];
    void *unk1c;
} UnkHeap_0201b9ec;

typedef struct {
    void *handle;
    UnkHeap_0201b9ec *heap;
    u8 pad8[0x2c - 8];
    u8 status;
    u8 pad2d;
    u8 pad2e;
    u8 pauseReq;
    u32 seqId;
    u8 pad34[8];
    u8 playerNo;
} UnkSndPlayer_0201b9ec;

void SND_PrepareSeq(u32 player);
void SND_SetTrackAllocatableChannel(u32 player, u32 mask, void *x);
void InitPlayer(UnkSndPlayer_0201b9ec *p);
u32 SND_GetCurrentCommandTag(void);

void NNSi_SndPlayerStartSeq(UnkSndPlayer_0201b9ec *p)
{
    UnkHeap_0201b9ec *heap = p->heap;
    SND_PrepareSeq(p->playerNo);
    if (heap->unk1c != NULL) {
        SND_SetTrackAllocatableChannel(p->playerNo, 0xffff, heap->unk1c);
    }
    InitPlayer(p);
    p->seqId = SND_GetCurrentCommandTag();
    p->pauseReq = TRUE;
    p->status = 1;
}
