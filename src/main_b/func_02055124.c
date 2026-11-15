#include <nitro/types.h>
#include <nitro/fs_overlay.h>

void func_02048e9c(void);
void func_0203051c(u32 overlayId, void (*fn)(void));
void func_02055124(void) { func_0203051c(FS_OVERLAY_ID(ov000), func_02048e9c); }
