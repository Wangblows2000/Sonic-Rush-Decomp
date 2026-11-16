#include <nitro/types.h>
#include <nitro/fs_overlay.h>

void TitleMenu_Unk_ov001_022d9530(void);
void Overlay_Request(u32 overlayId, void (*fn)(void));
void SceneChange_Unk_020550b4(void) { Overlay_Request(FS_OVERLAY_ID(ov001), TitleMenu_Unk_ov001_022d9530); }
