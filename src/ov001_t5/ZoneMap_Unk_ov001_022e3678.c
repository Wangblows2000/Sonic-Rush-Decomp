#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
    u32 flags;
    u8 pad30[0x48 - 0x30];
} UnkSprite_022e3678;

typedef struct {
    s32 timer;
    s32 visible;
    s32 run;
    UnkSprite_022e3678 spr[4];
} UnkStruct_022e3678;

void AnimatorSprite_ProcessAnimation(UnkSprite_022e3678 *s, s32 a, s32 b);

#pragma thumb on
void ZoneMap_Unk_ov001_022e3678(UnkStruct_022e3678 *p) {
    s32 i;

    if (p->visible != 0) {
        for (i = 0; i < 4; i++) {
            p->spr[i].flags |= 4;
            AnimatorSprite_ProcessAnimation(&p->spr[i], 0, 0);
        }
    }
    if (p->run != 0) {
        p->timer++;
    }
}
#pragma thumb reset
