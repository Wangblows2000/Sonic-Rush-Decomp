#include <nitro/types.h>
#include <nitro/fs_overlay.h>

void ExtraEnding_Create(void);
void Overlay_Request(u32 overlayId, void (*fn)(void));
void SceneChange_ToExtraEnding(void) { Overlay_Request(FS_OVERLAY_ID(ov001), ExtraEnding_Create); }
