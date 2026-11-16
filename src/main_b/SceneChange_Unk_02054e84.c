#include <nitro/types.h>
#include <nitro/fs_overlay.h>

void func_ov001_022f9eb4(void);
void Overlay_Request(u32 overlayId, void (*fn)(void));
void SceneChange_Unk_02054e84(void) { Overlay_Request(FS_OVERLAY_ID(ov001), func_ov001_022f9eb4); }
