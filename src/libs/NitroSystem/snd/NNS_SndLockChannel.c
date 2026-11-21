#include <nitro/types.h>

extern u32 data_02083448;
void SND_LockChannel(u32 a, u32 b);
BOOL NNS_SndLockChannel(u32 m) {
    if (m == 0) return 1;
    if (m & data_02083448) return 0;
    SND_LockChannel(m, 0);
    data_02083448 |= m;
    return 1;
}
