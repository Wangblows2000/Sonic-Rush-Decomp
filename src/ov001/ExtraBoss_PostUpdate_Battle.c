#include <nitro/types.h>

void ExtraBossObj_PropagateParentDestroy(void);
void ExtraBoss_UpdateCamera(void);
void ExtraBoss_UpdateBgWaveScroll(void);
void ExtraBoss_CollectRings(void);
void ExtraBoss_CheckAllCollisions(void);
void ExtraBossObj_DestroyFlagged(void);

void ExtraBoss_PostUpdate_Battle(void) {
    ExtraBossObj_PropagateParentDestroy();
    ExtraBoss_UpdateCamera();
    ExtraBoss_UpdateBgWaveScroll();
    ExtraBoss_CollectRings();
    ExtraBoss_CheckAllCollisions();
    ExtraBossObj_DestroyFlagged();
}
