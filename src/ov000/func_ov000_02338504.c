#include <nitro/types.h>

typedef struct UnkStruct_02338504 {
    u8 pad[0x10];
    u8 *unk10;
} UnkStruct_02338504;

extern UnkStruct_02338504 *gTaskCurrent;

void AnimatorSprite_ProcessAnimation(void *p, s32 a, s32 b);
void AnimatorSprite_Draw(void *p);

void func_ov000_02338504(void) {
    u8 *p = gTaskCurrent->unk10;
    AnimatorSprite_ProcessAnimation(p + 0, 0, 0);
    AnimatorSprite_Draw(p + 0);
}
