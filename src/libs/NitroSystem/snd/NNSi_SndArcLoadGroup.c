#include <nitro/types.h>

typedef struct { u8 type; u8 loadFlag; u16 pad; u32 index; } UnkGroupItem_0201e61c;
typedef struct { u32 count; UnkGroupItem_0201e61c items[1]; } UnkGroup_0201e61c;

UnkGroup_0201e61c *NNS_SndArcGetGroupInfo(s32 idx);
s32 NNSi_SndArcLoadSeq(s32 idx, u32 flags, void *heap, BOOL cache, void **out);
s32 NNSi_SndArcLoadSeqArc(s32 idx, u32 flags, void *heap, BOOL cache, void **out);
s32 NNSi_SndArcLoadBank(s32 idx, u32 flags, void *heap, BOOL cache, void **out);
s32 NNSi_SndArcLoadWaveArc(s32 idx, u32 flags, void *heap, BOOL cache, void **out);

s32 NNSi_SndArcLoadGroup(s32 idx, void *heap) {
    UnkGroup_0201e61c *g;
    u32 i;
    s32 r;
    g = NNS_SndArcGetGroupInfo(idx);
    if (g == NULL) return 1;
    for (i = 0; i < g->count; i++) {
        UnkGroupItem_0201e61c *it = &g->items[i];
        switch (it->type) {
        case 0:
            r = NNSi_SndArcLoadSeq(it->index, it->loadFlag, heap, TRUE, NULL);
            if (r != 0) return r;
            break;
        case 3:
            r = NNSi_SndArcLoadSeqArc(it->index, it->loadFlag, heap, TRUE, NULL);
            if (r != 0) return r;
            break;
        case 1:
            r = NNSi_SndArcLoadBank(it->index, it->loadFlag, heap, TRUE, NULL);
            if (r != 0) return r;
            break;
        case 2:
            r = NNSi_SndArcLoadWaveArc(it->index, it->loadFlag, heap, TRUE, NULL);
            if (r != 0) return r;
            break;
        }
    }
    return 0;
}
