#include <nitro/types.h>

extern u32 data_02083440;
BOOL NNS_SndLockCapture(u32 m) { if (m & data_02083440) return 0; data_02083440 |= m; return 1; }
