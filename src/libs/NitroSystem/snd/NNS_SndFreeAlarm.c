#include <nitro/types.h>

extern u32 data_02083444;
void NNS_SndFreeAlarm(u32 n) { data_02083444 &= ~(1 << n); }
