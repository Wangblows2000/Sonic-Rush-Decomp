#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x40];
} UnkSpr_022e3608;

typedef struct {
    s32 timer;
    u8 pad4[4];
    s32 blink;
    UnkSpr_022e3608 spr[4];
} UnkStruct_022e3608;

void AnimatorSprite_Draw(UnkSpr_022e3608 *s);

#pragma thumb on
void ZoneMap_DrawScrollArrows(UnkStruct_022e3608 *p) {
    if (p->blink == 0 || p->timer % 24 >= 6) {
        p->spr[0].x = 0;
        p->spr[0].y = 0x50;
        p->spr[1].x = 0x70;
        p->spr[1].y = 0;
        p->spr[2].x = 0xf0;
        p->spr[2].y = 0x50;
        p->spr[3].x = 0x70;
        p->spr[3].y = 0xb0;
        AnimatorSprite_Draw(&p->spr[0]);
        AnimatorSprite_Draw(&p->spr[1]);
        AnimatorSprite_Draw(&p->spr[2]);
        AnimatorSprite_Draw(&p->spr[3]);
    }
}
#pragma thumb reset
