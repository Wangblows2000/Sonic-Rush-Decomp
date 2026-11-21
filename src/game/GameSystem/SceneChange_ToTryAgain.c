#include <nitro/types.h>
#include <nitro/fs_overlay.h>

void TryAgain_Start(void);
void Overlay_Request(u32 overlayId, void (*fn)(void));
void SceneChange_ToTryAgain(void) { Overlay_Request(FS_OVERLAY_ID(ov001), TryAgain_Start); }
