#include <nitro/types.h>

typedef struct { u32 offset; u16 bankNo; u8 volume; u8 channelPrio; u8 playerPrio; u8 playerNo; } UnkSeqInfo_0201e9cc;

u32 *NNS_SndArcGetSeqArcInfo(s32 idx);
void *func_0201d0f8(u32 fileId);
UnkSeqInfo_0201e9cc *NNSi_SndSeqArcGetSeqInfo(void *data, s32 index);
BOOL StartSeqArc(void *handle, s32 playerNo, s32 bankNo, s32 prio, UnkSeqInfo_0201e9cc *info,
                   void *data, s32 a, s32 b);

BOOL func_0201e9cc(void *handle, s32 playerNo, s32 bankNo, s32 prio, s32 seqArcNo, s32 index) {
    u32 *arcInfo;
    void *data;
    UnkSeqInfo_0201e9cc *info;
    arcInfo = NNS_SndArcGetSeqArcInfo(seqArcNo);
    if (arcInfo == NULL) return FALSE;
    data = func_0201d0f8(*arcInfo);
    if (data == NULL) return FALSE;
    info = NNSi_SndSeqArcGetSeqInfo(data, index);
    if (info == NULL) return FALSE;
    if (prio < 0) prio = info->playerPrio;
    if (bankNo < 0) bankNo = info->bankNo;
    if (playerNo < 0) playerNo = info->playerNo;
    return StartSeqArc(handle, playerNo, bankNo, prio, info, data, seqArcNo, index);
}
