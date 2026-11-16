#include <nitro/types.h>

typedef struct {
    s32 unk0;
    u8 unk4[0xf8];
    u32 flags;
} UnkStruct_02084d38;

extern UnkStruct_02084d38 NNS_G3dGlb;
extern u8 data_02084df4[];
void NNS_G3dGeBufferOP_N(s32 a, void *b, s32 c);

void func_02038228(void) {
    UnkStruct_02084d38 *d = (UnkStruct_02084d38 *)((u32)&NNS_G3dGlb | 0);
    NNS_G3dGeBufferOP_N(d->unk0, d->unk4, 0x1e);
    NNS_G3dGeBufferOP_N(0x1b19, data_02084df4, 0xf);
    NNS_G3dGlb.flags &= ~1;
    NNS_G3dGlb.flags &= ~2;
}
