#include <nitro/types.h>
#include <main_b3/w50.h>

typedef struct { s32 x; s32 y; } UnkVec_020613cc;
typedef struct {
    u8 pad0[0x10];
    UnkSpr_W50 spr[2];     /* 0x10 */
    void *res[2];          /* 0xa0 */
    u16 pal[0x10];         /* 0xa8 */
    UnkVec_020613cc a[6];  /* 0xc8 */
    UnkVec_020613cc b[6];  /* 0xf8 */
    UnkVec_020613cc c[6];  /* 0x128 */
    u8 d[6];               /* 0x158 */
} UnkWork_020613cc;

void AnimatorSprite_ProcessAnimation(UnkSpr_W50 *s, u32 a, u32 b);
void AnimatorSprite_Draw(UnkSpr_W50 *s);

#pragma thumb on
void func_020613cc(UnkWork_020613cc *w) {
    u8 i;
    UnkSpr_W50 *spr;

    spr = &w->spr[0];
    AnimatorSprite_ProcessAnimation(spr, 0, 0);
    spr++;
    AnimatorSprite_ProcessAnimation(spr, 0, 0);
    for (i = 0; i < 6; i++) {
        w->b[i].x += w->c[i].x;
        w->b[i].y += w->c[i].y;
        w->a[i].x += w->b[i].x;
        w->a[i].y += w->b[i].y;
        spr = &w->spr[w->d[i]];
        spr->x = w->a[i].x >> 8;
        spr->y = w->a[i].y >> 8;
        AnimatorSprite_Draw(spr);
    }
}
#pragma thumb reset
