#include <nitro/types.h>
#include <nitro/fs_overlay.h>

void StaffRoll_StartExtra(void);
void Overlay_Request(u32 overlayId, void (*fn)(void));
void SceneChange_Unk_02054ebc(void) { Overlay_Request(FS_OVERLAY_ID(ov001), StaffRoll_StartExtra); }
