#include <nitro/types.h>

extern s32 gSpecialStageCamera[];
void func_ov001_02314c64(s32 i);
void SpecialStageCamera_FreeList(void);

void SpecialStageCamera_Shutdown(void) {
    func_ov001_02314c64(gSpecialStageCamera[13]);
    SpecialStageCamera_FreeList();
}
