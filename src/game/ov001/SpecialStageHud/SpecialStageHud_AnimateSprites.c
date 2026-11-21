#include <nitro/types.h>

typedef struct { u8 data[0x48]; } UnkStruct_02308ccc;
extern u8 *gSpecialStageHud;
void AnimatorSprite_ProcessAnimation(void *p, s32 a, s32 b);

void SpecialStageHud_AnimateSprites(void) {
    u8 *g = gSpecialStageHud;
    s32 i;
    for (i = 0; i < 0x17; i++) {
        AnimatorSprite_ProcessAnimation(&((UnkStruct_02308ccc *)(g + 0x108))[i], 0, 0);
    }
    AnimatorSprite_ProcessAnimation(g + 0x780, 0, 0);
    AnimatorSprite_ProcessAnimation(g + 0x7c8, 0, 0);
}
