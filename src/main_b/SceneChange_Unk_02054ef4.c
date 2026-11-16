#include <nitro/types.h>
#include <nitro/fs_overlay.h>

void func_ov001_022c66e8(void);
void Overlay_Request(u32 overlayId, void (*fn)(void));
void SceneChange_Unk_02054ef4(void) { Overlay_Request(FS_OVERLAY_ID(ov001), func_ov001_022c66e8); }
