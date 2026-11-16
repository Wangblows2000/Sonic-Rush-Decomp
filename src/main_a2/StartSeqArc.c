#include <nitro/types.h>

typedef struct { u32 offset; u16 bankNo; u8 volume; u8 channelPrio; } UnkSeqInfo_0201e7f0;
typedef struct { u8 unk0[0x18]; u32 dataOffset; } UnkSeqData_0201e7f0;

void *NNSi_SndPlayerAllocSeqPlayer(void *handle, u32 playerNo, s32 prio);
void *NNSi_SndPlayerAllocHeap(u32 playerNo, void *seq);
s32 NNSi_SndArcLoadBank(s32 idx, u32 flags, void *heap, BOOL cache, void **out);
void NNSi_SndPlayerFreeSeqPlayer(void *seq);
void NNSi_SndPlayerStartSeq(void *seq, void *data, u32 offset, void *bank);
void NNS_SndPlayerSetInitialVolume(void *handle, u8 v);
void NNS_SndPlayerSetChannelPriority(void *handle, u8 v);
void NNS_SndPlayerSetSeqArcNo(void *handle, u32 a, u32 b);

BOOL StartSeqArc(void *handle, u32 playerNo, s32 bankNo, s32 prio, UnkSeqInfo_0201e7f0 *info,
                   UnkSeqData_0201e7f0 *data, u32 a, u32 b) {
    void *seq;
    void *heap;
    void *bank;
    seq = NNSi_SndPlayerAllocSeqPlayer(handle, playerNo, prio);
    if (seq == NULL) return FALSE;
    heap = NNSi_SndPlayerAllocHeap(playerNo, seq);
    if (NNSi_SndArcLoadBank(bankNo, 6, heap, FALSE, &bank) != 0) {
        NNSi_SndPlayerFreeSeqPlayer(seq);
        return FALSE;
    }
    NNSi_SndPlayerStartSeq(seq, (u8 *)data + data->dataOffset, info->offset, bank);
    NNS_SndPlayerSetInitialVolume(handle, info->volume);
    NNS_SndPlayerSetChannelPriority(handle, info->channelPrio);
    NNS_SndPlayerSetSeqArcNo(handle, a, b);
    return TRUE;
}
