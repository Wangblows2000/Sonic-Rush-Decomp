#include <nitro/types.h>
#include <nitro/fs_overlay.h>

void func_ov001_022d301c(void);
void func_0203051c(u32 overlayId, void (*fn)(void));
void func_02054ed8(void) { func_0203051c(FS_OVERLAY_ID(ov001), func_ov001_022d301c); }
