#include <nitro/types.h>

typedef struct { u8 unk0[0x18]; u8 count; } UnkStruct_02025bfc;
void NNS_G3dMdlSetMdlAmb(UnkStruct_02025bfc *s, u32 idx, u32 v);

void NNS_G3dMdlSetMdlAmbAll(UnkStruct_02025bfc *s, u32 v) {
    u32 i;
    for (i = 0; i < s->count; i++) {
        NNS_G3dMdlSetMdlAmb(s, i, v);
    }
}
