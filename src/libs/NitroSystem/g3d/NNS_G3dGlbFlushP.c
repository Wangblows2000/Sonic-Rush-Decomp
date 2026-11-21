#include <nitro/types.h>

typedef struct { u8 unk0[0xFC]; u32 unkFC; } UnkStruct_0202247c;
extern UnkStruct_0202247c NNS_G3dGlb;
void NNS_G3dGeBufferOP_N(u32 a, void *b, u32 c);

void NNS_G3dGlbFlushP(void) {
    u32 *p = (u32 *)&NNS_G3dGlb;
    NNS_G3dGeBufferOP_N(*p++, p, 0x3E);
    NNS_G3dGlb.unkFC &= ~1;
    NNS_G3dGlb.unkFC &= ~2;
}
