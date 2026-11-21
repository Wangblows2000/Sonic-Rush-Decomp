#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x24];
    u32 unk2C;
    u8 pad30[0x18];
} UnkSpr_02047664;
typedef struct {
    u8 pad0[4];
    u8 *str;      /* 0x4 */
    u16 len;      /* 0x8 */
    u8 padA[2];
    s32 x;        /* 0xc */
    s32 y;        /* 0x10 */
    s16 scale[1]; /* 0x14 */
} UnkText_02047664;
typedef struct {
    u8 pad0[0x16c];
    UnkText_02047664 text;      /* 0x16c */
    u8 pad[0x39c - 0x16c - sizeof(UnkText_02047664)];
    UnkSpr_02047664 spr[0x33];  /* 0x39c */
} UnkWork_02047664;

extern u16 data_020733dc[];

void AnimatorSprite_Draw(UnkSpr_02047664 *s);
void AnimatorSprite_DrawRotScale(UnkSpr_02047664 *s, s32 sx, s32 sy, s32 r);

#pragma thumb on
void ClearDemoName_Draw(UnkWork_02047664 *w) {
    UnkText_02047664 *t = &w->text;
    s32 xo = 0;
    u8 c;
    UnkSpr_02047664 *spr;
    u32 i;

    for (i = 0; i < t->len; i++) {
        c = t->str[i];
        if (c != 0x13) {
            spr = &w->spr[c];
            spr->x = (t->x + xo) >> 12;
            spr->y = t->y >> 12;
            if (t->scale[i] != 0) {
                spr->unk2C &= ~0x60000;
                spr->unk2C |= 0x40000;
                if (t->scale[i] == 0x1000)
                    AnimatorSprite_Draw(spr);
                else
                    AnimatorSprite_DrawRotScale(spr, 0x1000, t->scale[i], 0);
                spr->unk2C &= ~0x60000;
            }
        }
        xo += data_020733dc[t->str[i]] << 12;
    }
}
#pragma thumb reset
