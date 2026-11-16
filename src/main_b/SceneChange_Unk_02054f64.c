#include <nitro/types.h>
#include <nitro/fs_overlay.h>

void func_ov001_022ec35c(void);
void Overlay_Request(u32 overlayId, void (*fn)(void));
void SceneChange_Unk_02054f64(void) { Overlay_Request(FS_OVERLAY_ID(ov001), func_ov001_022ec35c); }
