#include <nitro/types.h>

typedef struct { u8 unk0[0x94]; u32 unk94; } UnkStruct_0202224c;
extern UnkStruct_0202224c NNS_G3dGlb;

void NNS_G3dGlbMaterialColorDiffAmb(u32 a, u32 b, BOOL c) {
    NNS_G3dGlb.unk94 = a | (b << 16) | ((c ? 1 : 0) << 15);
}
