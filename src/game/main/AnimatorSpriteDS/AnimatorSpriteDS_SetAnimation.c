#include <nitro/types.h>

void AnimatorSprite_SetAnimation(void *a, void *b, void *c, void *d);

void AnimatorSpriteDS_SetAnimation(void *a, void *b, void *c, void *d) {
    AnimatorSprite_SetAnimation(a, b, c, d);
}
