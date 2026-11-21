#include <nitro/types.h>
#include <nitro/fs_overlay.h>

void ZoneMap_StartFromStage(void);
void Overlay_Request(u32 overlayId, void (*fn)(void));
void SceneChange_ToZoneMap_1_1(void) { Overlay_Request(FS_OVERLAY_ID(ov001), ZoneMap_StartFromStage); }
