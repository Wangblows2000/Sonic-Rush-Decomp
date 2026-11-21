#include <nitro/types.h>
#include <game/Graphics/animatorsprite.h>

typedef struct { u8 pad0[4]; u32 flags; } UnkPlayer_020576dc;
typedef struct {
    u8 pad0[4];
    UnkPlayer_020576dc *player; /* 0x4 */
    u8 pad8[0xc];
    UnkSpr_W50 spr[7];          /* 0x14 */
    s16 unk20C;                 /* 0x20c */
} UnkWork_020576dc;

void AnimatorSprite_ProcessAnimation(UnkSpr_W50 *s, u32 a, u32 b);
void AnimatorSprite_Draw(UnkSpr_W50 *s);

#pragma thumb on
void PolyDemoUnder2_DrawSprites(UnkWork_020576dc *w) {
    UnkSpr_W50 *spr;
    u32 f;

    spr = &w->spr[0];
    AnimatorSprite_ProcessAnimation(spr, 0, 0);
    AnimatorSprite_Draw(spr);
    spr = &w->spr[1];
    AnimatorSprite_ProcessAnimation(spr, 0, 0);
    AnimatorSprite_Draw(spr);
    spr = &w->spr[4];
    AnimatorSprite_ProcessAnimation(spr, 0, 0);
    AnimatorSprite_Draw(spr);
    f = w->player->flags;
    if (f & 0x100000) {
        if (f & 0x800000) {
            spr = &w->spr[3];
            AnimatorSprite_ProcessAnimation(spr, 0, 0);
            AnimatorSprite_Draw(spr);
            spr = &w->spr[6];
            AnimatorSprite_ProcessAnimation(spr, 0, 0);
            AnimatorSprite_Draw(spr);
        } else {
            spr = &w->spr[2];
            AnimatorSprite_ProcessAnimation(spr, 0, 0);
            AnimatorSprite_Draw(spr);
            spr = &w->spr[5];
            AnimatorSprite_ProcessAnimation(spr, 0, 0);
            w->unk20C = (w->unk20C + 1) % 60;
            if (w->unk20C < 0x28)
                AnimatorSprite_Draw(spr);
        }
    }
}
#pragma thumb reset
