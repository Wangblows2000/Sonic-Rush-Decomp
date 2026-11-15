#include <nitro/types.h>
#include <nitro/fs_overlay.h>

void func_ov001_022f9eb4(void);
void func_0203051c(u32 overlayId, void (*fn)(void));
void func_02054e84(void) { func_0203051c(FS_OVERLAY_ID(ov001), func_ov001_022f9eb4); }
