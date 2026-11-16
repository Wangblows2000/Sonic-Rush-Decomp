#include <nitro/types.h>
#include <nitro/fs_overlay.h>

void StageEntry_Start(void);
void Overlay_Request(u32 overlayId, void (*fn)(void));
void SceneChange_ToStage(void) { Overlay_Request(FS_OVERLAY_ID(ov000), StageEntry_Start); }
