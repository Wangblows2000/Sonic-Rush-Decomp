#include <nitro/types.h>
#include <nitro/fs_overlay.h>

void TitleMenu_StartAtPlayerSelectItem1(void);
void Overlay_Request(u32 overlayId, void (*fn)(void));
void SceneChange_ToTitleMenu_1_1(void) { Overlay_Request(FS_OVERLAY_ID(ov001), TitleMenu_StartAtPlayerSelectItem1); }
