#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
} UnkAnim_022eba70;

typedef struct {
    s32 a;
    s32 b;
    u8 pad8[0x20];
} UnkItem_022eba70;

typedef struct {
    s32 unk0;
    s32 unk4;
    UnkItem_022eba70 items[5];
    s32 unkd0;
    s32 unkd4;
    s32 unkd8;
    s32 unkdc;
    s32 unke0;
    u8 pade4[0xb5c - 0xe4];
    UnkAnim_022eba70 anims[12];
} UnkWork_022eba70;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022eba70 *work;
} UnkTask_022eba70;

extern UnkTask_022eba70 *gTaskCurrent;

void SysSound_PlaySE(s32 a);
void KeyframeAnim_SetAnim(UnkAnim_022eba70 *a, s32 id);
void TimeAttackMenu_Unk_ov001_022ea734(UnkWork_022eba70 *w);
void TimeAttackMenu_Unk_ov001_022ea6e4(UnkWork_022eba70 *w);
void TimeAttackMenu_Unk_ov001_022ea7c4(UnkWork_022eba70 *w);
void TimeAttackMenu_Unk_ov001_022ea644(UnkWork_022eba70 *w);
void TimeAttackMenu_IndexToGrid(UnkWork_022eba70 *w, s32 a, s32 *x, s32 *y);
void TimeAttackMenu_Unk_ov001_022ea5a8(UnkWork_022eba70 *w, s32 x);
void TimeAttackMenu_Unk_ov001_022ea504(UnkWork_022eba70 *w, s32 x, s32 y);
void TimeAttackMenu_GetRecordTimes(s32 x, s32 y, UnkItem_022eba70 *items);
void TimeAttackMenu_StartBg1HOfsHdma(UnkWork_022eba70 *w);
void TimeAttackMenu_FillHdmaBuffer(UnkWork_022eba70 *w);
void TimeAttackMenu_State_Select(void);
void TimeAttackMenu_State_Unk_ov001_022eae98(void);

#pragma thumb on
void TimeAttackMenu_Unk_ov001_022eba70(UnkTask_022eba70 *t) {
    UnkWork_022eba70 *w;
    s32 x;
    s32 y;
    s32 i;

    if (t == NULL) {
        t = gTaskCurrent;
    }
    w = t->work;
    w->unk0 = 0;
    if (w->unke0 != 0) {
        w->unkd4 = w->unkd0 - 1;
        if (w->unkd4 < 0) {
            w->unkd4 = w->unkd8 - 1;
        }
    } else {
        w->unkd4 = w->unkd0 + 1;
        if (w->unkd4 >= w->unkd8) {
            w->unkd4 = 0;
        }
    }
    if (w->unkd0 == w->unkd4) {
        t->func = TimeAttackMenu_State_Select;
        return;
    }
    SysSound_PlaySE(4);
    KeyframeAnim_SetAnim(&w->anims[0], 0);
    KeyframeAnim_SetAnim(&w->anims[1], 1);
    KeyframeAnim_SetAnim(&w->anims[2], 2);
    KeyframeAnim_SetAnim(&w->anims[3], 3);
    KeyframeAnim_SetAnim(&w->anims[4], 4);
    switch (w->unk4) {
    case 0:
    case 2: {
        s32 next = w->unkd4;
        s32 cur = w->unkd0;
        if (cur / 2 != next / 2 && (cur < 14 || next < 14)) {
            KeyframeAnim_SetAnim(&w->anims[5], 0xb);
        }
        break;
    }
    case 1:
    case 3:
        if (w->unkd0 != w->unkd4) {
            KeyframeAnim_SetAnim(&w->anims[5], 0xb);
        }
        break;
    }
    if (w->unke0 != 0) {
        TimeAttackMenu_Unk_ov001_022ea734(w);
        TimeAttackMenu_Unk_ov001_022ea6e4(w);
    } else {
        TimeAttackMenu_Unk_ov001_022ea7c4(w);
        TimeAttackMenu_Unk_ov001_022ea644(w);
    }
    TimeAttackMenu_IndexToGrid(w, w->unkd4, &x, &y);
    TimeAttackMenu_Unk_ov001_022ea5a8(w, x);
    TimeAttackMenu_Unk_ov001_022ea504(w, x, y);
    TimeAttackMenu_GetRecordTimes(x, y, w->items);
    for (i = 0; i < 5; i++) {
        w->items[i].a = 0x100;
        w->items[i].b = 0x100;
    }
    TimeAttackMenu_StartBg1HOfsHdma(w);
    TimeAttackMenu_FillHdmaBuffer(w);
    t->func = TimeAttackMenu_State_Unk_ov001_022eae98;
}
#pragma thumb reset
