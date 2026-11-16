#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x2c - 8];
    u32 flags;
    u8 pad30[0x44 - 0x30];
    u16 unk44;
    u8 pad46[2];
} UnkSpr_022e0d30;

typedef struct {
    u8 pad0[0xc];
    s32 count;
    u8 pad10[0xbc - 0x10];
    s32 y[(0x360 - 0xbc) / 4];
    UnkSpr_022e0d30 spr[(0xe10 - 0x360) / 0x48];
    s32 anim[8];
} UnkStruct_022e0d30;

extern s32 data_ov001_0231c3ec;

void AnimatorSprite_SetAnimation(UnkSpr_022e0d30 *s, u16 id);
void AnimatorSprite_ProcessAnimation(UnkSpr_022e0d30 *s, s32 a, s32 b);
void AnimatorSprite_Draw(UnkSpr_022e0d30 *s);

#pragma thumb on
void ZoneActSelect_Unk_ov001_022e0d30(UnkStruct_022e0d30 *p) {
    s32 i;
    UnkSpr_022e0d30 *s;

    for (i = 0; i < p->count; i++) {
        s = &p->spr[i];
        if (i != p->anim[i]) {
            AnimatorSprite_SetAnimation(s, i);
            p->anim[i] = i;
        }
        if (i == data_ov001_0231c3ec && p->y[i] == -8) {
            s->flags &= ~0x60000;
        } else {
            s->flags &= ~0x60000;
            s->flags |= 0x20000;
        }
        if (i < 3) {
            s->unk44 = 1;
        } else if (i < 6) {
            s->unk44 = 2;
        } else {
            s->unk44 = 3;
        }
        s->x = i * 0x20 + 0x10;
        s->y = p->y[i];
        AnimatorSprite_ProcessAnimation(s, 0, 0);
        AnimatorSprite_Draw(s);
    }
}
#pragma thumb reset
