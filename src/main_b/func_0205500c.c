#include <nitro/types.h>
#include <nitro/fs_overlay.h>

void func_ov001_022e8b20(void);
void func_0203051c(u32 overlayId, void (*fn)(void));
void func_0205500c(void) { func_0203051c(FS_OVERLAY_ID(ov001), func_ov001_022e8b20); }
