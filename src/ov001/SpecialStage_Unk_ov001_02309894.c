#include <nitro/types.h>

extern u8 *gSpecialStageHud;

BOOL SpecialStage_Unk_ov001_02309894(void) {
    return *(s32 *)(gSpecialStageHud + 0xbd4) >= 0;
}
