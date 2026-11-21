#include <nitro/types.h>

extern u32 data_02083444;
s32 NNS_SndAllocAlarm(void) { s32 i; u32 m = 1; for (i = 0; i < 8; i++) { if ((data_02083444 & m) == 0) { data_02083444 |= m; return i; } m <<= 1; } return -1; }
