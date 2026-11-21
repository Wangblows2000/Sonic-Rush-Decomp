#include <nitro/types.h>

extern s32 gSpecialStageCamera[];
void SpecialStageCamera_Unregister(s32 i);
void SpecialStageCamera_FreeList(void);

void SpecialStageCamera_Shutdown(void) {
    SpecialStageCamera_Unregister(gSpecialStageCamera[13]);
    SpecialStageCamera_FreeList();
}
