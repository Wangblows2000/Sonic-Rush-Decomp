#include <nitro/types.h>

typedef struct {
    u8 pad0[0x7c];
    s32 unk7c;
} UnkStruct_02084d38;

typedef struct {
    u32 unk0;
    u8 unk4[0x38];
} UnkStruct_02084db4;

extern UnkStruct_02084d38 NNS_G3dGlb;
extern UnkStruct_02084db4 data_02084db4;
extern u8 data_02084e30[];
void NNS_G3dGeBufferOP_N(s32 a, void *b, s32 c);

void Model_GlbSendLightMaterial(void) {
    s32 v;
    UnkStruct_02084db4 *d = (UnkStruct_02084db4 *)((u32)&data_02084db4 | 0);
    NNS_G3dGeBufferOP_N(NNS_G3dGlb.unk7c, d->unk4, 0xe);
    NNS_G3dGeBufferOP_N(0x2a, data_02084e30, 1);
    v = 2;
    NNS_G3dGeBufferOP_N(0x10, &v, 1);
}
