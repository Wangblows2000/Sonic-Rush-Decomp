#include <nitro/types.h>

u32 NNSi_SndArcLoadGroup(void *a, void *b, void *c, void *d);

BOOL NNS_SndArcLoadGroup(void *a, void *b, void *c, void *d) {
    return NNSi_SndArcLoadGroup(a, b, c, d) == 0;
}
