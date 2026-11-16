#include <nitro/types.h>
#include <nitro/fs_overlay.h>

void TitleMenu_Start(void);
void Overlay_Request(u32 overlayId, void (*fn)(void));
void SceneChange_Unk_020550ec(void) { Overlay_Request(FS_OVERLAY_ID(ov001), TitleMenu_Start); }
