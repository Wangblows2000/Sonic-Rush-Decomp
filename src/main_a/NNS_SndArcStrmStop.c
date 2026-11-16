#include <nitro/types.h>

void StopStrm(void *p);

void NNS_SndArcStrmStop(void **pp) {
    void *p = *pp;
    if (p == NULL) return;
    StopStrm(p);
}
