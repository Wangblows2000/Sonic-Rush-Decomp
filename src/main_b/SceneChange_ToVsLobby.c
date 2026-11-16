#include <nitro/types.h>
#include <nitro/fs_overlay.h>

void func_02063620(void);
void Overlay_Request(u32 overlayId, void (*fn)(void));
void SceneChange_ToVsLobby(void) { Overlay_Request(FS_OVERLAY_ID(ov000), func_02063620); }
