#include <nitro/types.h>

typedef struct { u32 offset; u16 bankNo; u8 volume; u8 channelPrio; u8 playerPrio; u8 playerNo; } UnkSeqInfo_0201e9cc;

u32 *func_0201d46c(s32 idx);
void *func_0201d0f8(u32 fileId);
UnkSeqInfo_0201e9cc *func_020204f4(void *data, s32 index);
BOOL func_0201e7f0(void *handle, s32 playerNo, s32 bankNo, s32 prio, UnkSeqInfo_0201e9cc *info,
                   void *data, s32 a, s32 b);

BOOL func_0201e9cc(void *handle, s32 playerNo, s32 bankNo, s32 prio, s32 seqArcNo, s32 index) {
    u32 *arcInfo;
    void *data;
    UnkSeqInfo_0201e9cc *info;
    arcInfo = func_0201d46c(seqArcNo);
    if (arcInfo == NULL) return FALSE;
    data = func_0201d0f8(*arcInfo);
    if (data == NULL) return FALSE;
    info = func_020204f4(data, index);
    if (info == NULL) return FALSE;
    if (prio < 0) prio = info->playerPrio;
    if (bankNo < 0) bankNo = info->bankNo;
    if (playerNo < 0) playerNo = info->playerNo;
    return func_0201e7f0(handle, playerNo, bankNo, prio, info, data, seqArcNo, index);
}
