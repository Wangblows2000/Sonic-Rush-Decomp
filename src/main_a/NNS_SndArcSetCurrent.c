#include <nitro/types.h>

extern u32 data_02083f04;

u32 NNS_SndArcSetCurrent(u32 v) {
    u32 old = data_02083f04;
    data_02083f04 = v;
    return old;
}
