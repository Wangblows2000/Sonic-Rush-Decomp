#include <nitro/types.h>

typedef struct { u8 unk0[0x98]; u32 unk98; } UnkStruct_02022228;
extern UnkStruct_02022228 NNS_G3dGlb;

void NNS_G3dGlbMaterialColorSpecEmi(u32 a, u32 b, BOOL c) {
    NNS_G3dGlb.unk98 = a | (b << 16) | ((c ? 1 : 0) << 15);
}
