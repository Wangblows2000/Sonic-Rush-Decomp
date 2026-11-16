#include <nitro/types.h>
#include <nitro/fs_overlay.h>

void func_02048e9c(void);
void Overlay_Request(u32 overlayId, void (*fn)(void));
void SceneChange_Unk_02055124(void) { Overlay_Request(FS_OVERLAY_ID(ov000), func_02048e9c); }
