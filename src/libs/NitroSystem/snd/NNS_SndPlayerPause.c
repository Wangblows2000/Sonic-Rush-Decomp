#include <nitro/types.h>

void NNSi_SndPlayerPause(void *p);
void NNS_SndPlayerPause(void **pp) { NNSi_SndPlayerPause(*pp); }
