#include <nitro/types.h>

void ExtraBossObj_PropagateParentDestroy(void);
void ExtraBoss_Unk_ov001_02304a24(void);
void ExtraBossObj_DestroyFlagged(void);

void ExtraBoss_Unk_ov001_022fef1c(void) {
    ExtraBossObj_PropagateParentDestroy();
    ExtraBoss_Unk_ov001_02304a24();
    ExtraBossObj_DestroyFlagged();
}
