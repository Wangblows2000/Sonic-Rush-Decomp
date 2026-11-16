#include <nitro/types.h>

BOOL NNS_SndArcStrmPrepare(void *p);
void NNS_SndArcStrmStartPrepared(void *p);

BOOL NNS_SndArcStrmStart(void *p) {
    if (!NNS_SndArcStrmPrepare(p)) return FALSE;
    NNS_SndArcStrmStartPrepared(p);
    return TRUE;
}
