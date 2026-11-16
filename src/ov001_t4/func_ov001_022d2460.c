#include <nitro/types.h>

typedef struct {
    u8 pad0[0x48];
} UnkSpr_022d2460;

typedef struct UnkWork_022d2460 UnkWork_022d2460;
struct UnkWork_022d2460 {
    u8 pad0[4];
    s32 unk4;
    u8 pad8[4];
    u8 unkc[4];
    void (*func10)(UnkWork_022d2460 *w);
    s32 unk14;
    u8 pad18[0x930 - 0x18];
    UnkSpr_022d2460 spr930;
    UnkSpr_022d2460 spr978;
    u8 pad9c0[0xbb0 - 0x9c0];
    s32 unkbb0;
    s32 unkbb4;
    s32 unkbb8;
    u8 padbbc[0xbd8 - 0xbbc];
    void (*funcbd8)(UnkWork_022d2460 *w);
    s32 unkbdc;
    u8 padbe0[0xcbc - 0xbe0];
    s32 pos[3];
    s32 pos2[3];
    s32 vel[3];
    s32 vel2[3];
    s32 acc[3];
    s32 acc2[3];
};

typedef struct {
    u8 pad0[0x10];
    UnkWork_022d2460 *work;
} UnkTask_022d2460;

typedef struct {
    u8 pad0[8];
    u16 x;
    u16 y;
    u8 padc[4];
    u32 flags;
} UnkTouch_022d2460;

typedef struct {
    u8 pad0[4];
    u16 trg;
} UnkPad_022d2460;

typedef struct {
    u8 pad0[8];
    s16 brightness;
} UnkBr_022d2460;

extern UnkTask_022d2460 *gTaskCurrent;
extern UnkTouch_022d2460 gTouchState;
extern UnkPad_022d2460 gPad;
extern UnkBr_022d2460 data_0208926c;
extern UnkBr_022d2460 data_02087d44;

void AnimatorSprite_ProcessAnimation(UnkSpr_022d2460 *s, s32 a, s32 b);
void AnimatorSprite_Draw(UnkSpr_022d2460 *s);
void AnimatorSprite_SetAnimation(UnkSpr_022d2460 *s, u16 anim);
s32 func_ov001_022cfb44(UnkSpr_022d2460 *s, u16 x, u16 y);
void func_02020204(void *p, s32 a);
void func_ov001_022d019c(UnkWork_022d2460 *w);
void func_ov001_022d1428(UnkWork_022d2460 *w, s32 a);
void func_ov001_022d2234(UnkWork_022d2460 *w, void (*f)(void));
void func_ov001_022d1c20(void);

#pragma thumb on
void func_ov001_022d2460(void) {
    UnkWork_022d2460 *w = gTaskCurrent->work;
    s32 hit;
    s32 i;

    w->unk4++;
    w->unk14++;
    if (w->unkbb0 == 1) {
        if (w->unkbb4 != 0 || (w->unk4 & 0x30)) {
            AnimatorSprite_ProcessAnimation(&w->spr930, 0, 0);
            AnimatorSprite_Draw(&w->spr930);
        }
        AnimatorSprite_ProcessAnimation(&w->spr978, 0, 0);
        AnimatorSprite_Draw(&w->spr978);
    }
    if (w->unk4 > 0x1e && w->unkbb0 == 1 && w->unkbb4 == 0) {
        hit = 0;
        if (gTouchState.flags & 0x40) {
            hit = func_ov001_022cfb44(&w->spr978, gTouchState.x, gTouchState.y);
        }
        if (hit == 1 || (gPad.trg & 8)) {
            w->unkbb4 = 1;
            w->unkbb8 = 0x3c;
            AnimatorSprite_SetAnimation(&w->spr930, 1);
            AnimatorSprite_SetAnimation(&w->spr978, 0x25);
            func_02020204(w->unkc, 0x12c);
        }
    }
    w->func10(w);
    w->unkbdc++;
    for (i = 0; i < 3; i++) {
        w->pos[i] += w->vel[i];
        w->pos2[i] += w->vel2[i];
        w->vel[i] += w->acc[i];
        w->vel2[i] += w->acc2[i];
    }
    w->funcbd8(w);
    func_ov001_022d019c(w);
    func_ov001_022d1428(w, 1);
    if (w->unkbb4 != 0) {
        w->unkbb8--;
        if (w->unkbb8 % 3 == 0) {
            data_0208926c.brightness--;
            data_02087d44.brightness--;
        }
        if (data_0208926c.brightness < -16) {
            data_0208926c.brightness = -16;
        }
        if (data_02087d44.brightness < -16) {
            data_02087d44.brightness = -16;
        }
        if (w->unkbb8 <= 0 && data_0208926c.brightness == -16 && data_02087d44.brightness == -16) {
            func_ov001_022d2234(w, func_ov001_022d1c20);
        }
    }
}
#pragma thumb reset
