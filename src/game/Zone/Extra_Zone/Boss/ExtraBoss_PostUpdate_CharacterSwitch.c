#include <nitro/types.h>

void ExtraBossObj_PropagateParentDestroy(void);
void ExtraBoss_UpdateBgWaveScroll(void);
void ExtraBossObj_DestroyFlagged(void);

void ExtraBoss_PostUpdate_CharacterSwitch(void) {
    ExtraBossObj_PropagateParentDestroy();
    ExtraBoss_UpdateBgWaveScroll();
    ExtraBossObj_DestroyFlagged();
}
