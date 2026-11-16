#include <nitro/types.h>
#include <nitro/fs_overlay.h>

void Ending_Create(void);
void Overlay_Request(u32 overlayId, void (*fn)(void));
void SceneChange_Unk_02054f10(void) { Overlay_Request(FS_OVERLAY_ID(ov001), Ending_Create); }
