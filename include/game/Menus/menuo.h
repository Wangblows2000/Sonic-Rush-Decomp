#ifndef OV000_D_MENUO_H
#define OV000_D_MENUO_H

#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x2C - 8];
    u32 unk2C;
    u8 pad30[0x48 - 0x30];
} UnkStruct_MenuOSpr;

/* ov000 0x02360xxx menu object: 21 sprites at 0x108 + current anim per sprite at 0x6FC */
typedef struct {
    s16 x;
    s16 y;
    s32 unk4;
    u8 pad8[0x2C - 8];
} UnkStruct_MenuOPos;

typedef struct {
    u32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    u8 pad20[0x24 - 0x20];
    u16 unk24;
    u8 pad26[2];
    s32 unk28;
    u8 pad2C[0x30 - 0x2C];
    UnkStruct_MenuOSpr spr30;
    UnkStruct_MenuOSpr spr78;
    UnkStruct_MenuOSpr sprC0;
    UnkStruct_MenuOSpr spr[21];
    s32 anim6F0;
    s32 anim6F4;
    s32 anim6F8;
    s32 anim[21];
    u8 pad750[0x850 - 0x750];
    UnkStruct_MenuOPos pos[18];
} UnkStruct_MenuO;

typedef struct {
    u8 pad0[8];
    void (*unk8)(void);
    u8 padC[4];
    UnkStruct_MenuO *work;
} UnkStruct_MenuOTask;

extern UnkStruct_MenuOTask *gTaskCurrent;

void KeyframeAnim_Update(UnkStruct_MenuOPos *p);
void KeyframeAnim_SetAnim(UnkStruct_MenuOPos *p, s32 n);
void TimeSelectMenu_DrawLayoutA(UnkStruct_MenuO *w);
void TimeSelectMenu_DrawLayoutB(UnkStruct_MenuO *w, s32 a, s32 b);

static inline void MenuO_Update(UnkStruct_MenuO *w, s32 b) {
    KeyframeAnim_Update(&w->pos[0]);
    TimeSelectMenu_DrawLayoutA(w);
    if (w->unkC / 2 != w->unk10 / 2) {
        TimeSelectMenu_DrawLayoutB(w, w->pos[0].unk4, b);
    } else {
        TimeSelectMenu_DrawLayoutB(w, 0x1000, b);
    }
    w->unk0++;
}

void AnimatorSprite_SetAnimation(UnkStruct_MenuOSpr *spr, u16 anim);

static inline void MenuO_SetAnim(UnkStruct_MenuOSpr *spr, s32 *cur, s32 a) {
    if (*cur != a) {
        AnimatorSprite_SetAnimation(spr, a);
        *cur = a;
    }
}
void AnimatorSprite_ProcessAnimation(UnkStruct_MenuOSpr *spr, s32 a, s32 b);
void AnimatorSprite_Draw(UnkStruct_MenuOSpr *spr);
void AnimatorSprite_DrawRotScale(UnkStruct_MenuOSpr *spr, s32 sx, s32 sy, s32 c);

#endif
