#include <nitro/types.h>
typedef float f32;

typedef struct UnkSpr_02307d30 {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x2c - 8];
    u32 flags;
    u8 pad30[0x46 - 0x30];
    u16 unk46;
} UnkSpr_02307d30;

typedef struct UnkPos_02307d30 {
    s16 x;
    s16 y;
} UnkPos_02307d30;

typedef struct UnkWork_02307d30 {
    u8 pad0[0x108];
    UnkSpr_02307d30 spr[35];
    u8 padAE0[0xbd4 - 0xae0];
    s32 unkBD4;
    u8 padBD8[0xbf8 - 0xbd8];
    s16 unkBF8;
    s16 unkBFA;
    UnkSpr_02307d30 spr2[3];
    s32 timer[3];
    UnkPos_02307d30 pos[3];
} UnkWork_02307d30;

typedef struct UnkBg_02307d30 {
    u16 x;
    u16 y;
} UnkBg_02307d30;

typedef struct UnkDisp_02307d30 {
    UnkBg_02307d30 bg[18];
    s16 bright;
} UnkDisp_02307d30;

extern UnkWork_02307d30 *gSpecialStageHud;
extern UnkDisp_02307d30 gDisplaySub;
extern UnkDisp_02307d30 gDisplayMain;
extern u8 data_ov001_0231708c[];
extern u16 data_ov001_02317114[];
extern u8 data_ov001_0231f5c8[];
extern u8 gSoundArchive[];

s32 SpecialStage_GetStageIndex(void);
void AnimatorSprite_ProcessAnimation(UnkSpr_02307d30 *s, s32 a, s32 b);
void AnimatorSprite_SetAnimation(UnkSpr_02307d30 *s, u16 anim);
void func_ov001_02307890(s32 i, s32 t);
void func_ov001_02307778(s32 a, f32 f, s16 *x, s16 *y);
BOOL func_0202e5e4(f32 a, f32 b);
void func_0201d568(void *p);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void func_ov001_02307990(s32 i);
void SpecialStage_RequestExit(s32 a);

void SpecialStageHud_UpdateClear(void) {
    UnkWork_02307d30 *w = gSpecialStageHud;
    UnkSpr_02307d30 *s21 = &w->spr[21];
    UnkSpr_02307d30 *s22 = &w->spr[22];
    s32 k;
    UnkSpr_02307d30 *sp;
    s32 i;
    s32 t;
    s16 b;
    f32 f;
    u8 idx;

    k = SpecialStage_GetStageIndex() + 0xb;
    w->unkBD4++;
    sp = &w->spr[k];
    if (w->unkBD4 <= 0x32) {
        b = 0x32 - w->unkBD4;
        if (b > 0x10)
            b = 0x10;
        else if (b < 0)
            b = 0;
        gDisplaySub.bright = b;
    }
    if (w->unkBD4 >= 0x2e4) {
        b = w->unkBD4 - 0x2e4;
        if (b > 0x10)
            b = 0x10;
        gDisplayMain.bright = b;
        gDisplaySub.bright = b;
    }
    if (w->unkBD4 >= 0x28 && w->unkBD4 <= 0x32) {
        s21->x = (w->unkBD4 << 8) / 10 - 0x100;
        s21->y = 0x20;
        AnimatorSprite_ProcessAnimation(s21, 0, 0);
    } else if (w->unkBD4 < 0x3c) {
        AnimatorSprite_ProcessAnimation(s21, 0, 0);
    } else if (w->unkBD4 >= 0x3c) {
        s22->x = 0;
        s22->y = 0x20;
        AnimatorSprite_ProcessAnimation(s22, 0, 0);
        idx = data_ov001_0231708c[3];
        gDisplaySub.bg[idx].x = w->unkBD4 - 0x32;
        gDisplaySub.bg[idx].y = 0xffe8;
    }
    if (w->unkBD4 >= 0x46) {
        t = w->unkBD4 - 0x46;
        for (i = 0; i < 7; i++)
            func_ov001_02307890(i, t);
        if (w->unkBD4 >= 0x17c) {
            f = (f32)(w->unkBD4 - 0x17c) / 60.0f;
            if (func_0202e5e4(f, 1.0f))
                f = 1.0f;
            func_ov001_02307778(SpecialStage_GetStageIndex(), 50.0f * f, &w->unkBF8, &w->unkBFA);
        }
    }
    if (w->unkBD4 == 0xfc) {
        AnimatorSprite_SetAnimation(sp, data_ov001_02317114[k] + 1);
        sp->unk46 = 3;
        func_0201d568(data_ov001_0231f5c8);
        Sound_PlaySeqArc(0, 0xe, 3);
        func_0201d568(gSoundArchive);
    } else if (w->unkBD4 > 0xfc && (sp->flags & 2)) {
        AnimatorSprite_SetAnimation(sp, data_ov001_02317114[k]);
    }
    AnimatorSprite_ProcessAnimation(sp, 0, 0);
    for (i = 0; i < 3; i++) {
        AnimatorSprite_ProcessAnimation(&w->spr2[i], 0, 0);
        if (w->timer[i] > 0) {
            w->pos[i].y++;
            w->spr2[i].x = w->pos[i].x;
            w->spr2[i].y = w->pos[i].y;
            w->timer[i]--;
        } else {
            func_ov001_02307990(i);
        }
    }
    if (w->unkBD4 == 0x302)
        SpecialStage_RequestExit(1);
}
