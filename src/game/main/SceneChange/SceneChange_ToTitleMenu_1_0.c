#include <nitro/types.h>
#include <nitro/fs_overlay.h>

void TitleMenu_StartAtPlayerSelectItem0(void);
void Overlay_Request(u32 overlayId, void (*fn)(void));
void SceneChange_ToTitleMenu_1_0(void) { Overlay_Request(FS_OVERLAY_ID(ov001), TitleMenu_StartAtPlayerSelectItem0); }
