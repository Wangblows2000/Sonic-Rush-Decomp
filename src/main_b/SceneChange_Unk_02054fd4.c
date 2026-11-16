#include <nitro/types.h>
#include <nitro/fs_overlay.h>

void ZoneMap_StartDefault(void);
void Overlay_Request(u32 overlayId, void (*fn)(void));
void SceneChange_Unk_02054fd4(void) { Overlay_Request(FS_OVERLAY_ID(ov001), ZoneMap_StartDefault); }
