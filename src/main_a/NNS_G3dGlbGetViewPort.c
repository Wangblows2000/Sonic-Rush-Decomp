#include <nitro/types.h>

typedef struct { u8 unk0[0xA0]; u32 unkA0; } UnkStruct_0202205c;
extern UnkStruct_0202205c NNS_G3dGlb;

void NNS_G3dGlbGetViewPort(u32 *a, u32 *b, u32 *c, u32 *d) {
    if (a != NULL) *a = NNS_G3dGlb.unkA0 & 0xFF;
    if (b != NULL) *b = (NNS_G3dGlb.unkA0 >> 8) & 0xFF;
    if (c != NULL) *c = (NNS_G3dGlb.unkA0 >> 16) & 0xFF;
    if (d != NULL) *d = (NNS_G3dGlb.unkA0 >> 24) & 0xFF;
}
