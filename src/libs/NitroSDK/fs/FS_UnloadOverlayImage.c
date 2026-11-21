#include <nitro/types.h>

void FS_EndOverlay(void *p);
BOOL FS_UnloadOverlayImage(void *p) { FS_EndOverlay(p); return 1; }
