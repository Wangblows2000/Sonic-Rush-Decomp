#include <nitro/types.h>

typedef struct { u8 unk0[0x18]; u8 count; } UnkStruct_02025bb4;
void NNS_G3dMdlSetMdlEmi(UnkStruct_02025bb4 *s, u32 idx, u32 v);

void NNS_G3dMdlSetMdlEmiAll(UnkStruct_02025bb4 *s, u32 v) {
    u32 i;
    for (i = 0; i < s->count; i++) {
        NNS_G3dMdlSetMdlEmi(s, i, v);
    }
}
