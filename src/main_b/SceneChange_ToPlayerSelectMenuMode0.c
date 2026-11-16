#include <nitro/types.h>
#include <nitro/fs_overlay.h>

void func_ov000_0235a5d0(void);
void Overlay_Request(u32 overlayId, void (*fn)(void));
void SceneChange_ToPlayerSelectMenuMode0(void) { Overlay_Request(FS_OVERLAY_ID(ov000), func_ov000_0235a5d0); }
