#include <nitro/types.h>
#include <nitro/fs_overlay.h>

void TitleScreen_Start(void);
void Overlay_Request(u32 overlayId, void (*fn)(void));
void SceneChange_ToTitleScreen(void) { Overlay_Request(FS_OVERLAY_ID(ov001), TitleScreen_Start); }
