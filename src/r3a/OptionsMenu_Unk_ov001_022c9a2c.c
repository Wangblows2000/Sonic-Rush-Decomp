#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x40];
} UnkSpr_022c9a2c;

typedef struct {
    u8 pad0[0xc0c];
    UnkSpr_022c9a2c spr[41];
    u32 anim[41];
} UnkWork_022c9a2c;

typedef struct {
    s32 mode;
    u8 pad4[0x20];
    s32 scroll;
    u8 pad28[0x10];
    s32 text0[41];
    s32 text1[41];
} UnkTicker_022c9a2c;

typedef struct {
    u8 pad0[0x38];
    UnkTicker_022c9a2c t;
    u8 pad[0x3ac - 0x38 - sizeof(UnkTicker_022c9a2c)];
    UnkWork_022c9a2c *work;
} UnkStruct_022c9a2c;

extern s32 data_ov001_02316028[];

void AnimatorSprite_SetAnimation(UnkSpr_022c9a2c *s, u16 anim);
void AnimatorSprite_ProcessAnimation(UnkSpr_022c9a2c *s, u32 a, u32 b);
void AnimatorSprite_Draw(UnkSpr_022c9a2c *s);

#pragma thumb on
void OptionsMenu_Unk_ov001_022c9a2c(UnkStruct_022c9a2c *p, s32 x, s32 y) {
    UnkWork_022c9a2c *w = p->work;
    UnkTicker_022c9a2c *t = &p->t;
    s32 left = x - 0x27;
    s32 *text;
    s32 i;
    s32 n;
    s32 width;
    s32 s;
    u16 anim;
    s32 pos;
    s16 cx;
    UnkSpr_022c9a2c *spr;

    if (t->mode == 0)
        text = t->text0;
    else
        text = t->text1;
    n = 0;
    width = 0;
    for (i = 0; i < 41; i++) {
        if (text[i] == 0x2e)
            break;
        width += data_ov001_02316028[text[i]];
        n++;
    }
    width += 0x10;
    if (width < 0x5e)
        width = 0x5e;
    s = t->scroll - 0x10;
    if (s < 0)
        s = 0;
    s <<= 0xb;
    if (s > 0 && s % (width << 12) == 0)
        t->scroll = 0x10;
    pos = left - (s >> 12) % width;
    cx = pos;
    while (pos < left + 0x4e) {
        for (i = 0; i < n; i++) {
            spr = &w->spr[i];
            if (cx >= left - 8) {
                anim = text[i];
                if (anim != w->anim[i]) {
                    AnimatorSprite_SetAnimation(spr, anim);
                    w->anim[i] = anim;
                }
                spr->x = cx;
                spr->y = y;
                AnimatorSprite_ProcessAnimation(spr, 0, 0);
                AnimatorSprite_Draw(spr);
            }
            cx += data_ov001_02316028[text[i]];
            if (cx >= left + 0x4e)
                break;
        }
        pos += width;
        cx = pos;
    }
}
#pragma thumb reset
