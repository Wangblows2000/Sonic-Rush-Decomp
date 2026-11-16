#include <nitro/types.h>
#include <nitro/fs_overlay.h>

void func_ov001_022e8b08(void);
void Overlay_Request(u32 overlayId, void (*fn)(void));
void SceneChange_Unk_02054fd4(void) { Overlay_Request(FS_OVERLAY_ID(ov001), func_ov001_022e8b08); }
