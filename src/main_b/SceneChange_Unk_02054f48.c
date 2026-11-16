#include <nitro/types.h>
#include <nitro/fs_overlay.h>

void TimeAttackMenu_StartMode3(void);
void Overlay_Request(u32 overlayId, void (*fn)(void));
void SceneChange_Unk_02054f48(void) { Overlay_Request(FS_OVERLAY_ID(ov001), TimeAttackMenu_StartMode3); }
