#include <nitro/types.h>
#include <nitro/fs_overlay.h>

void func_ov001_02300378(void);
void Overlay_Request(u32 overlayId, void (*fn)(void));
void SceneChange_Unk_02054ea0(void) { Overlay_Request(FS_OVERLAY_ID(ov001), func_ov001_02300378); }
