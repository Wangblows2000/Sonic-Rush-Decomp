#include <nitro/types.h>

typedef struct {
    s16 x;
    s16 y;
    s32 sx;
    s32 sy;
    u8 padc[0x20];
} UnkAnim_022ea894;

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x40];
} UnkSprite_022ea894;

typedef struct {
    u8 pad0[8];
    s32 unk8;
    u8 padc[0x1c];
} UnkItem_022ea894;

typedef struct {
    u32 unk0;
    u32 unk4;
    UnkItem_022ea894 items[5];
    s32 unkd0;
    s32 unkd4;
    s32 unkd8;
    s32 unkdc;
    s32 unke0;
    u8 pade4[0x7c8 - 0xe4];
    UnkSprite_022ea894 sprites[11];
    u8 padae0[0xb5c - 0xae0];
    UnkAnim_022ea894 anims[12];
} UnkWork_022ea894;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022ea894 *work;
} UnkTask_022ea894;

extern UnkTask_022ea894 *gTaskCurrent;

void KeyframeAnim_Update(UnkAnim_022ea894 *a);
BOOL KeyframeAnim_IsFinished(UnkAnim_022ea894 *a);
void TimeAttackMenu_Unk_ov001_022ea360(UnkWork_022ea894 *w, s32 i, s32 v);
void TimeAttackMenu_Unk_ov001_022e9fe4(UnkWork_022ea894 *w, s32 i);
void TimeAttackMenu_Unk_ov001_022e9fa8(UnkWork_022ea894 *w, s32 i);
void TimeAttackMenu_FillHdmaBuffer(UnkWork_022ea894 *w);
void AnimatorSprite_ProcessAnimation(void *p, s32 a, s32 b);
void AnimatorSprite_Draw(void *p);
void AnimatorSprite_DrawRotScale(void *p, s32 sx, s32 sy, s32 d);
BOOL TimeAttackMenu_IsConfirmPressed(UnkWork_022ea894 *w);
BOOL TimeAttackMenu_IsLeftPressed(UnkWork_022ea894 *w);
BOOL TimeAttackMenu_IsRightPressed(UnkWork_022ea894 *w);
void TimeAttackMenu_IndexToGrid(UnkWork_022ea894 *w, s32 a, s32 *x, s32 *y);
void Task_DestroyByTag(s32 a);
void TimeAttackMenu_StartStage(u32 id, s32 x, s32 y);
void TimeAttackMenu_Unk_ov001_022ea850(UnkWork_022ea894 *w);
void TimeAttackMenu_Unk_ov001_022ea504(UnkWork_022ea894 *w, s32 x, s32 y);
void TimeAttackMenu_Unk_ov001_022ea77c(UnkWork_022ea894 *w);
void TimeAttackMenu_Unk_ov001_022ea694(UnkWork_022ea894 *w);
void TimeAttackMenu_Unk_ov001_022eba08(s32 a);
void TimeAttackMenu_Unk_ov001_022eb994(s32 a);
BOOL TimeAttackMenu_IsBackPressed(UnkWork_022ea894 *w);

#pragma thumb on
void TimeAttackMenu_Unk_ov001_022ea894(void) {
    UnkWork_022ea894 *w = gTaskCurrent->work;
    UnkAnim_022ea894 *a;
    s32 i;
    UnkAnim_022ea894 *an;
    UnkItem_022ea894 *it;

    if (w->unk0 >= 16) {
        u32 id = w->unk4;
        Task_DestroyByTag(0x1000);
        ZoneMapPause_ExecuteChoice(id);
        return;
    }
    KeyframeAnim_Update(&w->anims[5]);
    KeyframeAnim_Update(&w->anims[6]);
    KeyframeAnim_Update(&w->anims[7]);
    KeyframeAnim_Update(&w->anims[8]);
    KeyframeAnim_Update(&w->anims[9]);
    KeyframeAnim_Update(&w->anims[10]);
    KeyframeAnim_Update(&w->anims[11]);
    i = 0;
    it = w->items;
    for (; i < 5; i++) {
        TimeAttackMenu_Unk_ov001_022ea360(w, i, 0);
        it->unk8 = 8;
        it->unk8 += i * 0x18;
        TimeAttackMenu_Unk_ov001_022e9fe4(w, i);
        TimeAttackMenu_Unk_ov001_022e9fa8(w, i);
        it++;
    }
    a = &w->anims[6];
    if (w->unkd0 >= 14) {
        w->sprites[10].x = a->x;
        w->sprites[10].y = a->y;
        AnimatorSprite_ProcessAnimation(&w->sprites[10], 0, 0);
        AnimatorSprite_Draw(&w->sprites[10]);
    } else {
        w->sprites[5].x = a->x;
        w->sprites[5].y = a->y;
        AnimatorSprite_ProcessAnimation(&w->sprites[5], 0, 0);
        AnimatorSprite_Draw(&w->sprites[5]);
    }
    if (w->unk4 <= 1) {
        a = &w->anims[7];
        w->sprites[0].x = a->x;
        w->sprites[0].y = a->y;
        AnimatorSprite_ProcessAnimation(&w->sprites[0], 0, 0);
        AnimatorSprite_Draw(&w->sprites[0]);
    }
    a = &w->anims[8];
    w->sprites[1].x = a->x;
    w->sprites[1].y = a->y;
    AnimatorSprite_ProcessAnimation(&w->sprites[1], 0, 0);
    AnimatorSprite_Draw(&w->sprites[1]);
    a = &w->anims[9];
    w->sprites[2].x = a->x;
    w->sprites[2].y = a->y;
    AnimatorSprite_ProcessAnimation(&w->sprites[2], 0, 0);
    AnimatorSprite_Draw(&w->sprites[2]);
    a = &w->anims[5];
    if (w->unkd0 >= 14) {
        w->sprites[9].x = a->x;
        w->sprites[9].y = a->y;
        AnimatorSprite_ProcessAnimation(&w->sprites[9], 0, 0);
        if (a->sx == 0x1000 && a->sy == 0x1000) {
            AnimatorSprite_Draw(&w->sprites[9]);
        } else {
            AnimatorSprite_DrawRotScale(&w->sprites[9], a->sx, a->sy, 0);
        }
    } else {
        w->sprites[3].x = a->x;
        w->sprites[3].y = a->y;
        AnimatorSprite_ProcessAnimation(&w->sprites[3], 0, 0);
        if (a->sx == 0x1000 && a->sy == 0x1000) {
            AnimatorSprite_Draw(&w->sprites[3]);
        } else {
            AnimatorSprite_DrawRotScale(&w->sprites[3], a->sx, a->sy, 0);
        }
    }
    a = &w->anims[10];
    w->sprites[8].x = a->x;
    w->sprites[8].y = a->y;
    AnimatorSprite_ProcessAnimation(&w->sprites[8], 0, 0);
    AnimatorSprite_Draw(&w->sprites[8]);
    a = &w->anims[11];
    w->sprites[4].x = a->x;
    w->sprites[4].y = a->y;
    AnimatorSprite_ProcessAnimation(&w->sprites[4], 0, 0);
    AnimatorSprite_Draw(&w->sprites[4]);
    if (w->unk0 == 8) {
        TimeAttackMenu_Unk_ov001_022ea41c(w);
    }
    w->unk0++;
}
#pragma thumb reset
