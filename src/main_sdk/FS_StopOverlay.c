#include <nitro/types.h>

void FS_EndOverlay(void *p);
BOOL FS_StopOverlay(void *p) { FS_EndOverlay(p); return 1; }
