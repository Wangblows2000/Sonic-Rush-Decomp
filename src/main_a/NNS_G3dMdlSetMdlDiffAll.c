#include <nitro/types.h>

typedef struct { u8 unk0[0x18]; u8 count; } UnkStruct_02025c44;
void NNS_G3dMdlSetMdlDiff(UnkStruct_02025c44 *s, u32 idx, u32 v);

void NNS_G3dMdlSetMdlDiffAll(UnkStruct_02025c44 *s, u32 v) {
    u32 i;
    for (i = 0; i < s->count; i++) {
        NNS_G3dMdlSetMdlDiff(s, i, v);
    }
}
