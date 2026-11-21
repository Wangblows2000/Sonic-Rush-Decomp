#include <nitro/types.h>

void AnimatorSprite_AdvanceFrames(void *p);
void AnimatorSprite_ProcessFrame(void *p);

void AnimatorSprite_AnimateManual(void *p) {
    AnimatorSprite_AdvanceFrames(p);
    AnimatorSprite_ProcessFrame(p);
}
