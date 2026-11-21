#include <nitro/types.h>

u8 *NNS_SndArcGetSeqInfo(void *a, void *b, void *c, void *d);

void *NNS_SndArcGetSeqParam(void *a, void *b, void *c, void *d) {
    u8 *r = NNS_SndArcGetSeqInfo(a, b, c, d);
    if (r == NULL) return NULL;
    return r + 4;
}
