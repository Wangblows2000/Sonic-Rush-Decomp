#include <nitro/types.h>

void func_ov001_022d9590(void);
/* overlay IDs are linker-defined symbols (NitroSDK FS_EXTERN_OVERLAY convention) */
#include <nitro/fs_overlay.h>
FS_EXTERN_OVERLAY(ov000);
FS_EXTERN_OVERLAY(ov001);
void func_0203051c(u32 overlayId, void (*fn)(void));
void func_020550d0(void) { func_0203051c(FS_OVERLAY_ID(ov001), func_ov001_022d9590); }
