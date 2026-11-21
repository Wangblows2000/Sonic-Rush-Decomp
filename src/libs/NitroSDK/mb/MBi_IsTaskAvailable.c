#include <nitro/types.h>

extern void *data_02080f40;
BOOL MBi_IsTaskAvailable(void) { return data_02080f40 != 0 ? 1 : 0; }
