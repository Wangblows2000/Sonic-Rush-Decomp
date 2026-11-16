#include <nitro/types.h>
#include <nitro/fs_overlay.h>

void func_0204d834(void);
void Overlay_Request(u32 overlayId, void (*fn)(void));
void SceneChange_Unk_02055178(void) { Overlay_Request(FS_OVERLAY_ID(ov000), func_0204d834); }
