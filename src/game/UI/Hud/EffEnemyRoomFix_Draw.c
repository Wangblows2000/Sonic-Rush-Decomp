#include <nitro/types.h>

typedef struct UnkStruct_02328928 {
    u8 pad[0x10];
    u8 *unk10;
} UnkStruct_02328928;

extern UnkStruct_02328928 *gTaskCurrent;

void AnimatorSprite_ProcessAnimation(void *p, s32 a, s32 b);
void AnimatorSprite_Draw(void *p);

void EffEnemyRoomFix_Draw(void) {
    u8 *p = gTaskCurrent->unk10;
    AnimatorSprite_ProcessAnimation(p + 0x94, 0, 0);
    AnimatorSprite_Draw(p + 0x94);
}
