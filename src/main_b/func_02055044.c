#include <nitro/types.h>
#include <nitro/fs_overlay.h>

void func_ov000_0235a558(void);
void func_0203051c(u32 overlayId, void (*fn)(void));
void func_02055044(void) { func_0203051c(FS_OVERLAY_ID(ov000), func_ov000_0235a558); }
