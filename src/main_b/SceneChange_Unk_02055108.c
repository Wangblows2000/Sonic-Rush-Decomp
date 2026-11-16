#include <nitro/types.h>
#include <nitro/fs_overlay.h>

void SpecialStage_Start(void);
void Overlay_Request(u32 overlayId, void (*fn)(void));
void SceneChange_Unk_02055108(void) { Overlay_Request(FS_OVERLAY_ID(ov001), SpecialStage_Start); }
