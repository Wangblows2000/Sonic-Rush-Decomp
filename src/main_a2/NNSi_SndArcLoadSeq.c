#include <nitro/types.h>

typedef struct { u32 fileId; u16 bankNo; } UnkSeqInfo_0201e570;

UnkSeqInfo_0201e570 *NNS_SndArcGetSeqInfo(s32 idx);
s32 NNSi_SndArcLoadBank(s32 idx, u32 flags, void *heap, BOOL cache, void **out);
void *LoadSeq(u32 fileId, void *heap, BOOL cache);
void *func_0201d0f8(u32 fileId);

s32 NNSi_SndArcLoadSeq(s32 idx, u32 flags, void *heap, BOOL cache, void **out) {
    UnkSeqInfo_0201e570 *info;
    void *p;
    s32 r;
    info = NNS_SndArcGetSeqInfo(idx);
    if (info == NULL) return 2;
    r = NNSi_SndArcLoadBank(info->bankNo, flags, heap, cache, NULL);
    if (r != 0) return r;
    if (flags & 1) {
        p = LoadSeq(info->fileId, heap, cache);
        if (p == NULL) return 6;
    } else {
        p = func_0201d0f8(info->fileId);
    }
    if (out != NULL) *out = p;
    return 0;
}
