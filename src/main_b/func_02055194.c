#include <nitro/types.h>
#include <nitro/fs_overlay.h>

void func_0204d7e0(void);
void func_0203051c(u32 overlayId, void (*fn)(void));
void func_02055194(void) { func_0203051c(FS_OVERLAY_ID(ov000), func_0204d7e0); }
