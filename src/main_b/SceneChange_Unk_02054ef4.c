#include <nitro/types.h>
#include <nitro/fs_overlay.h>

void ExtraEnding_Create(void);
void Overlay_Request(u32 overlayId, void (*fn)(void));
void SceneChange_Unk_02054ef4(void) { Overlay_Request(FS_OVERLAY_ID(ov001), ExtraEnding_Create); }
