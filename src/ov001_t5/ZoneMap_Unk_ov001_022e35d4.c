#include <nitro/types.h>

typedef struct {
    u8 pad0[0x48];
} UnkSprite_022e35d4;

typedef struct {
    u8 pad0[4];
    s32 mode;
    u8 pad8[4];
    UnkSprite_022e35d4 spr[4];
} UnkStruct_022e35d4;

void AnimatorSprite_SetAnimation(void *s, u16 id);
void AnimatorSprite_ProcessAnimation(void *s, s32 a, s32 b);

#pragma thumb on
void ZoneMap_Unk_ov001_022e35d4(UnkStruct_022e35d4 *p, s32 mode) {
    s32 i;

    if (p->mode != mode) {
        p->mode = mode;
        for (i = 0; i < 4; i++) {
            AnimatorSprite_SetAnimation(&p->spr[i], i);
            AnimatorSprite_ProcessAnimation(&p->spr[i], 0, 0);
        }
    }
}
#pragma thumb reset
