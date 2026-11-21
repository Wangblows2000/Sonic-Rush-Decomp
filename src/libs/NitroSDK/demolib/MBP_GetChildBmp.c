#include <nitro/types.h>

extern u16 *sMbpChildBmpTable[];
u16 MBP_GetChildBmp(s32 i) { return *sMbpChildBmpTable[i]; }
