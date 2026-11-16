#include <nitro/types.h>

u16 SpecialStage_PipeXToAngle(s32 v);

void SpecialStagePlayer_UpdateAngleFromX(u8 *p) {
    *(u16 *)(p + 0x16) = SpecialStage_PipeXToAngle(*(s32 *)(p + 0x20));
}
