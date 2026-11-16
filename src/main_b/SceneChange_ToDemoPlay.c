#include <nitro/types.h>
#include <nitro/fs_overlay.h>

void DemoPlay_Start(void);
void Overlay_Request(u32 overlayId, void (*fn)(void));
void SceneChange_ToDemoPlay(void) { Overlay_Request(FS_OVERLAY_ID(ov000), DemoPlay_Start); }
