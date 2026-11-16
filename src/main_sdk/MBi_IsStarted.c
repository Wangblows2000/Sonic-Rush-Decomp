#include <nitro/types.h>

extern u8 *data_02080f18;
BOOL MBi_IsStarted(void) { return data_02080f18[0x50d] == 1; }
