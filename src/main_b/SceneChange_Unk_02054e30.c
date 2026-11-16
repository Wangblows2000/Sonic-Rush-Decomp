#include <nitro/types.h>
#include <nitro/fs_overlay.h>

void PostStaffRoll_Start(void);
void Overlay_Request(u32 overlayId, void (*fn)(void));
void SceneChange_Unk_02054e30(void) { Overlay_Request(FS_OVERLAY_ID(ov001), PostStaffRoll_Start); }
