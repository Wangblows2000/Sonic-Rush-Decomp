#include <nitro/types.h>

typedef struct { u8 seqMax; u8 pad; u16 allocChBitFlag; u32 heapSize; } UnkPlayerInfo_0201ebec;

u32 func_0201d558(void);
UnkPlayerInfo_0201ebec *NNS_SndArcGetPlayerInfo(s32 idx);
void NNS_SndPlayerSetPlayableSeqCount(s32 idx, u8 v);
void NNS_SndPlayerSetAllocatableChannel(s32 idx, u16 v);
BOOL NNS_SndPlayerCreateHeap(s32 idx, void *heap, u32 size);

BOOL NNS_SndArcPlayerSetup(void *heap) {
    s32 i;
    func_0201d558();
    for (i = 0; i < 0x20; i++) {
        UnkPlayerInfo_0201ebec *info = NNS_SndArcGetPlayerInfo(i);
        if (info == NULL) continue;
        NNS_SndPlayerSetPlayableSeqCount(i, info->seqMax);
        NNS_SndPlayerSetAllocatableChannel(i, info->allocChBitFlag);
        if (info->heapSize != 0 && heap != NULL) {
            s32 j;
            for (j = 0; j < info->seqMax; j++) {
                if (!NNS_SndPlayerCreateHeap(i, heap, info->heapSize)) return FALSE;
            }
        }
    }
    return TRUE;
}
