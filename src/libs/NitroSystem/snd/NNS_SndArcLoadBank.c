#include <nitro/types.h>

u32 NNSi_SndArcLoadBank(void *a, u32 b, u32 c, u32 d, u32 e);

BOOL NNS_SndArcLoadBank(void *a, u32 b) {
    return NNSi_SndArcLoadBank(a, 0xFF, b, 1, 0) == 0;
}
