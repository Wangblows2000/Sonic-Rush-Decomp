#include <nitro/types.h>

extern u32 data_02084de0[];

void NNS_G3dGlbLightColor(u32 index, u32 value) {
    data_02084de0[index] = value | (index << 30);
}
