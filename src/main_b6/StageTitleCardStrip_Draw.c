#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    u8 x4[4];
    s16 h8;
    s16 ha;
    u8 padc[0x40];
    s32 x4c;
    s32 x50;
} UnkSpr_0205d46c;

typedef struct {
    u8 pad0[0x88];
    UnkSpr_0205d46c spr;
    u8 pad[0x488 - 0x88 - sizeof(UnkSpr_0205d46c)];
    u32 flags;
} UnkWork_0205d46c;

void AnimatorSprite_Draw(void *s);

void StageTitleCardStrip_Draw(UnkWork_0205d46c *w) {
    UnkSpr_0205d46c *s = &w->spr;

    if ((w->flags & 0x10) && s->x4c <= -0x100000)
        s->x4c += 0x100000;
    s->h8 = s->x4c >> 12;
    s->ha = s->x50 >> 12;
    AnimatorSprite_Draw(s->x4);
    if ((w->flags & 0x10) == 0)
        return;
    s->h8 += 0x100;
    AnimatorSprite_Draw(s->x4);
}
