#include <nitro/types.h>

void NNS_G3dGeBufferOP_N(u32 a, void *b, u32 c);

typedef struct { u32 flags; u8 unk4[0x24]; u8 unk28[0x24]; u8 unk4C[0xC]; } UnkStruct_02028528;

void NNSi_G3dSendJointSRTBasic(UnkStruct_02028528 *s) {
    if (!(s->flags & 4)) {
        if (!(s->flags & 2)) {
            NNS_G3dGeBufferOP_N(0x19, s->unk28, 0xC);
        } else {
            NNS_G3dGeBufferOP_N(0x1C, s->unk4C, 3);
        }
    } else if (!(s->flags & 2)) {
        NNS_G3dGeBufferOP_N(0x1A, s->unk28, 9);
    }
    if (s->flags & 1) return;
    NNS_G3dGeBufferOP_N(0x1B, s->unk4, 3);
}
