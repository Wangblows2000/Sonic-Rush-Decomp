#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x24];
    u32 flags;
    u8 pad30[0x18];
} UnkSpr_022ca710;

typedef struct {
    u8 pad0[0x7e4];
    UnkSpr_022ca710 sprites[14];
    s32 ids[14];
} UnkWork_022ca710;

typedef struct {
    s32 unk0;
    u8 pad4[0x10];
    s32 unk14;
    u8 pad18[4];
    s32 unk1c;
    u8 pad20[8];
    s32 unk28;
    s32 unk2c;
    s32 unk30;
    s32 unk34;
} UnkSub_022ca710;

typedef struct {
    u8 pad0[0x1c];
    s32 unk1c;
    u8 pad20[0x18];
    UnkSub_022ca710 sub;
    u8 pad70[0x3ac - 0x70];
    UnkWork_022ca710 *work;
} UnkStruct_022ca710;

void AnimatorSprite_SetAnimation(UnkSpr_022ca710 *s, u16 anim);
void AnimatorSprite_ProcessAnimation(UnkSpr_022ca710 *s, u32 a, u32 b);
void AnimatorSprite_Draw(UnkSpr_022ca710 *s);
void OptionsMenu_DrawPageBackButton(UnkStruct_022ca710 *p, s16 x, s16 y, s32 a);
void OptionsMenu_Unk_ov001_022c9f54(UnkStruct_022ca710 *p, s16 x, s16 y, s32 a, s32 b);
void OptionsMenu_Unk_ov001_022c9d58(UnkStruct_022ca710 *p, s16 x, s16 y, s32 a, s32 b);
void OptionsMenu_Unk_ov001_022c9b78(UnkStruct_022ca710 *p, s16 x, s16 y);
void OptionsMenu_DrawSoundTestTrackName(UnkStruct_022ca710 *p, s16 x, s16 y, s32 a);
void OptionsMenu_DrawSoundTestTrackNumber(UnkStruct_022ca710 *p, s16 x, s16 y, s32 a);

#pragma thumb on
void OptionsMenu_DrawPage3(UnkStruct_022ca710 *p, s16 x, s16 y) {
    UnkWork_022ca710 *w;
    UnkSub_022ca710 *sub;
    UnkSpr_022ca710 *s;
    u16 a1;
    u32 *f3;
    s32 ty;
    UnkSpr_022ca710 *s2;
    UnkSpr_022ca710 *s1;
    UnkSpr_022ca710 *s4;
    UnkSpr_022ca710 *s3;
    u16 a2;
    u32 *f4;
    UnkSpr_022ca710 *s13;

    if (x < 0x100) {
        w = p->work;
        sub = &p->sub;
        s = &w->sprites[0];
        if (w->ids[0] != 0x18) {
            AnimatorSprite_SetAnimation(s, 0x18);
            w->ids[0] = 0x18;
        }
        s->x = x + 0x18;
        s->y = y + 0x18;
        AnimatorSprite_ProcessAnimation(s, 0, 0);
        AnimatorSprite_Draw(s);
        if (sub->unk0 != 0) {
            OptionsMenu_DrawPageBackButton(p, x, y, 2);
        } else if (p->unk1c >= 0) {
            OptionsMenu_DrawPageBackButton(p, x, y, 1);
        } else {
            OptionsMenu_DrawPageBackButton(p, x, y, 0);
        }
        if (sub->unk0 != 0) {
            if (sub->unk30 < sub->unk28) {
                sub->unk30 = sub->unk28;
            } else {
                sub->unk30 -= 0x80;
                if (sub->unk30 < 0x1000) {
                    sub->unk30 = 0x1000;
                }
            }
            if (sub->unk34 < sub->unk2c) {
                sub->unk34 = sub->unk2c;
            } else {
                sub->unk34 -= 0x80;
                if (sub->unk34 < 0x1000) {
                    sub->unk34 = 0x1000;
                }
            }
        } else {
            sub->unk30 -= 0x80;
            if (sub->unk30 < 0x1000) {
                sub->unk30 = 0x1000;
            }
            sub->unk34 -= 0x80;
            if (sub->unk34 < 0x1000) {
                sub->unk34 = 0x1000;
            }
        }
        sub->unk28 = 0x1000;
        sub->unk2c = 0x1000;
        {
            s32 sc = sub->unk30;
            ty = y + 0x5c;
            OptionsMenu_Unk_ov001_022c9f54(p, x + 0x36, ty, sc, sc);
        }
        OptionsMenu_Unk_ov001_022c9d58(p, x + 0xca, ty, sub->unk34, sub->unk34);
        OptionsMenu_Unk_ov001_022c9b78(p, x, y);
        OptionsMenu_DrawSoundTestTrackName(p, x + 0x80, y + 0x49, sub->unk1c);
        OptionsMenu_DrawSoundTestTrackNumber(p, x + 0x68, y + 0x4c, sub->unk14);
        s3 = &w->sprites[3];
        f3 = &s3->flags;
        if (w->ids[3] != 4) {
            AnimatorSprite_SetAnimation(s3, 4);
            w->ids[3] = 4;
        }
        s3->x = x;
        s3->y = y;
        *f3 |= 4;
        AnimatorSprite_ProcessAnimation(s3, 0, 0);
        AnimatorSprite_Draw(s3);
        s4 = &w->sprites[4];
        f4 = &s4->flags;
        if (w->ids[4] != 5) {
            AnimatorSprite_SetAnimation(s4, 5);
            w->ids[4] = 5;
        }
        s4->x = x;
        s4->y = y;
        *f4 |= 4;
        AnimatorSprite_ProcessAnimation(s4, 0, 0);
        AnimatorSprite_Draw(s4);
        s1 = &w->sprites[1];
        a1 = 0;
        if (sub->unk0 != 0) {
            a1++;
        }
        if (w->ids[1] != a1) {
            AnimatorSprite_SetAnimation(s1, a1);
            w->ids[1] = a1;
        }
        s1->x = x;
        s1->y = y;
        AnimatorSprite_ProcessAnimation(s1, 0, 0);
        AnimatorSprite_Draw(s1);
        s2 = &w->sprites[2];
        a2 = 2;
        if (sub->unk0 == 0) {
            a2++;
        }
        if (w->ids[2] != a2) {
            AnimatorSprite_SetAnimation(s2, a2);
            w->ids[2] = a2;
        }
        s2->x = x;
        s2->y = y;
        AnimatorSprite_ProcessAnimation(s2, 0, 0);
        AnimatorSprite_Draw(s2);
        s13 = &w->sprites[13];
        if (w->ids[13] != 0) {
            AnimatorSprite_SetAnimation(s13, 0);
            w->ids[13] = 0;
        }
        s13->x = x;
        s13->y = y;
        AnimatorSprite_ProcessAnimation(s13, 0, 0);
        AnimatorSprite_Draw(s13);
    }
}
#pragma thumb reset
