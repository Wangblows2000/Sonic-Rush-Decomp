#include <nitro/types.h>
#include <nitro/fs_overlay.h>

void PlayerSelectMenu_StartMode2(void);
void Overlay_Request(u32 overlayId, void (*fn)(void));
void SceneChange_ToPlayerSelectMenuMode2(void) { Overlay_Request(FS_OVERLAY_ID(ov000), PlayerSelectMenu_StartMode2); }
