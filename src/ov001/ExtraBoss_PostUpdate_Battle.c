#include <nitro/types.h>

void ExtraBossObj_PropagateParentDestroy(void);
void ExtraBoss_UpdateCamera(void);
void ExtraBoss_Unk_ov001_02304a24(void);
void ExtraBoss_Unk_ov001_023041a8(void);
void ExtraBoss_Unk_ov001_022fea6c(void);
void ExtraBossObj_DestroyFlagged(void);

void ExtraBoss_PostUpdate_Battle(void) {
    ExtraBossObj_PropagateParentDestroy();
    ExtraBoss_UpdateCamera();
    ExtraBoss_Unk_ov001_02304a24();
    ExtraBoss_Unk_ov001_023041a8();
    ExtraBoss_Unk_ov001_022fea6c();
    ExtraBossObj_DestroyFlagged();
}
