#include <nitro/types.h>

typedef struct { u32 fileId : 24; u32 flags : 8; } UnkInfo_0201e2f8;

UnkInfo_0201e2f8 *NNS_SndArcGetWaveArcInfo(s32 idx);
void *LoadWaveArcTable(u32 fileId, void *heap, BOOL cache);
void *LoadWaveArc(u32 fileId, void *heap, BOOL cache);
void *NNS_SndArcGetFileAddress(u32 fileId);

s32 NNSi_SndArcLoadWaveArc(s32 idx, u32 flags, void *heap, BOOL cache, void **out) {
    UnkInfo_0201e2f8 *info = NNS_SndArcGetWaveArcInfo(idx);
    void *p;
    if (info == NULL) return 5;
    if (flags & 4) {
        if (info->flags & 1) {
            p = LoadWaveArcTable(info->fileId, heap, cache);
        } else {
            p = LoadWaveArc(info->fileId, heap, cache);
        }
        if (p == NULL) return 9;
    } else {
        p = NNS_SndArcGetFileAddress(info->fileId);
    }
    if (out != NULL) *out = p;
    return 0;
}
