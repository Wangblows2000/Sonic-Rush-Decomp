#include <nitro/types.h>

typedef struct { u8 unk0[0x18]; u8 count; } UnkStruct_02025b6c;
void NNS_G3dMdlSetMdlAlpha(UnkStruct_02025b6c *s, u32 idx, u32 v);

void NNS_G3dMdlSetMdlAlphaAll(UnkStruct_02025b6c *s, u32 v) {
    u32 i;
    for (i = 0; i < s->count; i++) {
        NNS_G3dMdlSetMdlAlpha(s, i, v);
    }
}
