#include <nitro/types.h>

void FS_NotifyArchiveAsyncEnd(u32 a, u32 b);
void FSi_OnRomReadDone(u32 a) { FS_NotifyArchiveAsyncEnd(a, 0); }
