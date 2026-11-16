#include <nitro/types.h>
#include <nitro/fs_overlay.h>

void TransformDemo_Create(void);
void Overlay_Request(u32 overlayId, void (*fn)(void));
void SceneChange_Unk_02054f2c(void) { Overlay_Request(FS_OVERLAY_ID(ov001), TransformDemo_Create); }
