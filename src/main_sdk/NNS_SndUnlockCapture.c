#include <nitro/types.h>

extern u32 data_02083440;
void NNS_SndUnlockCapture(u32 m) { data_02083440 &= ~m; }
