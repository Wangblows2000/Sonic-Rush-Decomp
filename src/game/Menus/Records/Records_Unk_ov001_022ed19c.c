#include <nitro/types.h>

typedef struct UnkSpr_022ed19c {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x48 - 8];
} UnkSpr_022ed19c;

typedef struct UnkStruct_022ed19c {
    u8 pad0[4];
    s32 unk4;
    s32 unk8;
    u8 padC[0x2c - 0xc];
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    u8 pad38[0x64 - 0x38];
    UnkSpr_022ed19c spr[21];
    u32 ids[21];
} UnkStruct_022ed19c;

void AnimatorSprite_SetAnimation(UnkSpr_022ed19c *s, u16 anim);
void AnimatorSprite_ProcessAnimation(UnkSpr_022ed19c *s, u32 a, u32 b);
void AnimatorSprite_Draw(UnkSpr_022ed19c *s);
s32 _s32_div_f(s32 a, s32 b);

#pragma thumb on
void Records_Unk_ov001_022ed19c(UnkStruct_022ed19c *p) {
    UnkSpr_022ed19c *s;
    u32 *id;
    u32 anim;
    s32 a;
    s32 e;
    s32 f;

    if (p->unk2C == 0)
        return;

    s = &p->spr[12];
    id = &p->ids[12];
    if (*id != 0x10) {
        *id = 0x10;
        AnimatorSprite_SetAnimation(s, *id);
    }
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);

    s = &p->spr[10];
    id = &p->ids[10];
    if (p->unk8 == 0)
        anim = 0xc;
    else if (p->unk30 > 0)
        anim = 0xb;
    else
        anim = 0xa;
    if (*id != anim) {
        *id = anim;
        AnimatorSprite_SetAnimation(s, *id);
    }
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);

    s = &p->spr[11];
    id = &p->ids[11];
    if (p->unk8 == 1)
        anim = 0xf;
    else if (p->unk30 < p->unk34)
        anim = 0xe;
    else
        anim = 0xd;
    if (*id != anim) {
        *id = anim;
        AnimatorSprite_SetAnimation(s, *id);
    }
    if (*id != anim) {
        *id = anim;
        AnimatorSprite_SetAnimation(s, *id);
    }
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);

    a = _s32_div_f(p->unk30, 2);
    e = a + (0x57 - _s32_div_f(p->unk34, 2));
    f = e - 8;

    s = &p->spr[13];
    id = &p->ids[13];
    anim = 0x11;
    if (p->unk4 == 4)
        anim++;
    if (*id != anim) {
        *id = anim;
        AnimatorSprite_SetAnimation(s, *id);
    }
    s->x = 0xd8;
    s->y = a + 0x25;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);

    s = &p->spr[14];
    id = &p->ids[14];
    anim = 0x15;
    if (p->unk4 == 4)
        anim++;
    if (*id != anim) {
        *id = anim;
        AnimatorSprite_SetAnimation(s, *id);
    }
    s->x = 0xd8;
    s->y = f + 0x25;
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    AnimatorSprite_Draw(s);

    s = &p->spr[15];
    id = &p->ids[15];
    anim = 0x13;
    if (p->unk4 == 4)
        anim++;
    if (*id != anim) {
        *id = anim;
        AnimatorSprite_SetAnimation(s, *id);
    }
    AnimatorSprite_ProcessAnimation(s, 0, 0);
    for (; a < f; a += 8) {
        s->x = 0xd8;
        s->y = a + 0x25;
        AnimatorSprite_Draw(s);
    }
}
#pragma thumb reset
