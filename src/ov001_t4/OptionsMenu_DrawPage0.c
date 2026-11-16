#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x24];
    u32 flags;
    u8 pad30[0x16];
    u16 unk46;
} UnkSpr_022cadbc;

typedef struct {
    s32 unk0;
    u8 pad4[0x10];
    s32 unk14;
    u8 pad18[0x584 - 0x18];
    UnkSpr_022cadbc sprites[8];
    s32 ids[8];
} UnkWork_022cadbc;

typedef struct {
    u8 pad0[0x14];
    u32 unk14;
    u8 pad18[4];
    s32 unk1c;
    u8 pad20[0xc];
    s32 unk2c;
    s32 unk30;
    s32 unk34;
    u8 pad38[0x3ac - 0x38];
    UnkWork_022cadbc *work;
} UnkStruct_022cadbc;

void AnimatorSprite_SetAnimation(UnkSpr_022cadbc *s, u16 anim);
void AnimatorSprite_ProcessAnimation(UnkSpr_022cadbc *s, u32 a, u32 b);
void AnimatorSprite_Draw(UnkSpr_022cadbc *s);
void OptionsMenu_Unk_ov001_022ca2d4(UnkStruct_022cadbc *p, s16 x, s16 y, s32 a);
void OptionsMenu_Unk_ov001_022ca1cc(UnkStruct_022cadbc *p, s32 x, s32 y, s32 a);
void OptionsMenu_Unk_ov001_022ca150(UnkStruct_022cadbc *p, s32 x, s32 y, s32 a);

#pragma thumb on
void OptionsMenu_DrawPage0(UnkStruct_022cadbc *p, s16 x, s16 y) {
    UnkWork_022cadbc *w;
    s32 has;
    UnkSpr_022cadbc *s;
    u16 anim;
    s32 f;

    if (x < 0x100) {
        w = p->work;
        has = w->unk14;
        s = &w->sprites[0];
        if (w->ids[0] != 0x15) {
            AnimatorSprite_SetAnimation(s, 0x15);
            w->ids[0] = 0x15;
        }
        s->x = x + 0x18;
        s->y = y + 0x18;
        AnimatorSprite_ProcessAnimation(s, 0, 0);
        AnimatorSprite_Draw(s);
        if (p->unk1c >= 0) {
            OptionsMenu_Unk_ov001_022ca2d4(p, x, y, 1);
        } else {
            OptionsMenu_Unk_ov001_022ca2d4(p, x, y, 0);
        }
        s = &w->sprites[3];
        anim = 0x1c;
        if (has) {
            s->x = x + 0x38;
            s->y = y + 0x40;
        } else {
            s->x = x + 0x38;
            s->y = y + 0x58;
        }
        if (p->unk2c == 0 && p->unk30 == 0) {
            if (p->unk34 == p->unk30) {
                anim += 2;
                s->flags |= 4;
            } else {
                anim += 1;
                s->flags &= ~4;
                f = p->unk14 < 4 ? 1 : 0;
                if (p->work->unk0 != 8) {
                    OptionsMenu_Unk_ov001_022ca1cc(p, s->x, s->y, f);
                }
            }
        }
        if (w->ids[3] != anim) {
            AnimatorSprite_SetAnimation(s, anim);
            w->ids[3] = anim;
        }
        s->unk46 = 4;
        AnimatorSprite_ProcessAnimation(s, 0, 0);
        AnimatorSprite_Draw(s);
        if (has) {
            s = &w->sprites[4];
            anim = 0x1f;
            s->x = x + 0x38;
            s->y = y + 0x58;
            if (p->unk2c == 0 && p->unk30 == 1) {
                anim += 1;
                OptionsMenu_Unk_ov001_022ca1cc(p, s->x, s->y, p->unk14 < 4 ? 1 : 0);
            } else if (p->unk2c == 1) {
                OptionsMenu_Unk_ov001_022ca1cc(p, s->x, s->y, 2);
            }
            if (w->ids[4] != anim) {
                AnimatorSprite_SetAnimation(s, anim);
                w->ids[4] = anim;
            }
            s->unk46 = 4;
            AnimatorSprite_ProcessAnimation(s, 0, 0);
            AnimatorSprite_Draw(s);
        }
        if (p->unk2c == 1) {
            s = &w->sprites[5];
            anim = 0x22;
            s->x = x + 0x38;
            s->y = y + 0x70;
            if (p->unk30 == 0) {
                if (p->unk34 == p->unk30) {
                    anim += 2;
                    s->flags |= 4;
                } else {
                    anim += 1;
                    s->flags &= ~4;
                    f = p->unk14 < 4 ? 1 : 0;
                    if (p->work->unk0 != 8) {
                        OptionsMenu_Unk_ov001_022ca150(p, s->x, s->y, f);
                    }
                }
            }
            if (w->ids[5] != anim) {
                AnimatorSprite_SetAnimation(s, anim);
                w->ids[5] = anim;
            }
            s->unk46 = 4;
            AnimatorSprite_ProcessAnimation(s, 0, 0);
            AnimatorSprite_Draw(s);
            s = &w->sprites[6];
            anim = 0x25;
            s->x = x + 0x80;
            s->y = y + 0x70;
            if (p->unk30 == 1) {
                if (p->unk34 == p->unk30) {
                    anim += 2;
                    s->flags |= 4;
                } else {
                    anim += 1;
                    s->flags &= ~4;
                    f = p->unk14 < 4 ? 1 : 0;
                    if (p->work->unk0 != 8) {
                        OptionsMenu_Unk_ov001_022ca150(p, s->x, s->y, f);
                    }
                }
            }
            if (w->ids[6] != anim) {
                AnimatorSprite_SetAnimation(s, anim);
                w->ids[6] = anim;
            }
            s->unk46 = 4;
            AnimatorSprite_ProcessAnimation(s, 0, 0);
            AnimatorSprite_Draw(s);
        }
        s = &w->sprites[7];
        if (w->ids[7] != 0x34) {
            AnimatorSprite_SetAnimation(s, 0x34);
            w->ids[7] = 0x34;
        }
        s->x = x + 0x18;
        s->y = y + 0x98;
        AnimatorSprite_ProcessAnimation(s, 0, 0);
        AnimatorSprite_Draw(s);
    }
}
#pragma thumb reset
