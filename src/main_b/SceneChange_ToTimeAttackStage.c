#include <nitro/types.h>
#include <nitro/fs_overlay.h>

void StageEntry_StartTimeAttack(void);
void Overlay_Request(u32 overlayId, void (*fn)(void));
void SceneChange_ToTimeAttackStage(void) { Overlay_Request(FS_OVERLAY_ID(ov000), StageEntry_StartTimeAttack); }
