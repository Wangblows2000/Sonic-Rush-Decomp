#include <nitro/types.h>

void NNSi_SndPlayerStopSeq(void *p);

void NNS_SndPlayerStopSeq(void **pp) {
    NNSi_SndPlayerStopSeq(*pp);
}
