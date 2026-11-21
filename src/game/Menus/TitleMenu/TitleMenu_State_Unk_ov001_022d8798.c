#include <nitro/types.h>

typedef struct {
    s32 id;
    u32 flags;
    s16 x;
    s16 y;
    u8 padc[0x30];
} UnkItem_022d8798;

typedef struct {
    u8 pad0[4];
    s32 sel;
    u32 timer;
    s32 unkc;
    u8 obj10[2];
    s16 y0;
    u8 pad14[2];
    s16 dy;
    u8 pad18[0xc];
    UnkItem_022d8798 items[6];
    void *res18c;
    u8 pad190[4];
    void *res194[4];
    void *sw;
} UnkWork_022d8798;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022d8798 *work;
} UnkTask_022d8798;

extern UnkTask_022d8798 *gTaskCurrent;

void TitleMenu_DrawBackButton(s32 x, s32 y, s32 a, s32 b);
void TitleMenu_Unk_ov001_022d7ac4(s32 id, s32 x, s32 y);
void TitleMenu_DrawItemButton(s32 idx, s16 x, s16 y, s32 mode, u32 t);
BOOL MsgBox_IsClosed(void);
BOOL MsgBox_GetResult(void);
void TitleMenu_RestoreSubDisplayThunk(void *sw);
void TitleMenu_ShowMsgBox0(void *sw);
void TitleMenu_StartFadeToWhite(s32 a, s32 b);
void TitleMenu_StartFadeToBlack(s32 a, s32 b);
void TitleMenu_Unk_ov001_022d7e60(void *p, void *sw);
void TitleMenu_CreateBgSlideTaskForward(void *sw, void *a, void *b);
void SysSound_PlaySEDelayed(s32 a, s32 b, s32 c, s32 d);
void TitleMenu_State_Unk_ov001_022d8938(void);
void TitleMenu_State_Unk_ov001_022d85d0(void);

#pragma thumb on
#pragma opt_common_subs off
void TitleMenu_State_Unk_ov001_022d8798(void) {
    UnkWork_022d8798 *w;
    u32 t;
    s16 y;
    s32 i;
    UnkItem_022d8798 *it;
    UnkItem_022d8798 *si;
    BOOL snd;

    w = gTaskCurrent->work;
    t = w->timer;
    y = w->y0;

    i = 0;
    it = w->items;
    for (; i < 6; i++) {
        if (it->id != 0x14) {
            if (i == 4) {
                TitleMenu_DrawBackButton(it->x, it->y, 0, 0);
            } else if (i == 5) {
                TitleMenu_Unk_ov001_022d7ac4(it->id, it->x, it->y);
            } else {
                if (w->sel == i) {
                    if (w->unkc != 0) {
                        TitleMenu_DrawItemButton(it->id, it->x, y, 3, t);
                    } else {
                        TitleMenu_DrawItemButton(it->id, it->x, y, 2, t);
                    }
                } else {
                    TitleMenu_DrawItemButton(it->id, it->x, y, 0, 0);
                }
                y += w->dy;
            }
        }
        it++;
    }
    if (w->unkc != 0) {
        if (!MsgBox_IsClosed()) {
            return;
        }
        TitleMenu_RestoreSubDisplayThunk(w->sw);
        if (MsgBox_GetResult()) {
            w->timer = 0;
            w->unkc = 0;
            gTaskCurrent->func = TitleMenu_State_Unk_ov001_022d8938;
            return;
        }
    }
    w->timer++;
    if (t >= 30) {
        si = &w->items[w->sel];
        if ((si->flags & 4) && w->unkc == 0) {
            w->unkc = 1;
            TitleMenu_ShowMsgBox0(w->sw);
            return;
        }
        snd = FALSE;
        w->unkc = 0;
        if (si->flags & 1) {
            TitleMenu_StartFadeToWhite(8, 0x14);
            snd = TRUE;
        }
        if (si->flags & 2) {
            TitleMenu_StartFadeToBlack(8, 0x14);
            snd = TRUE;
        }
        TitleMenu_Unk_ov001_022d7e60(w->obj10, w->sw);
        TitleMenu_CreateBgSlideTaskForward(w->sw, w->res18c, w->res194[w->sel]);
        if (snd) {
            SysSound_PlaySEDelayed(2, 0x14, 0x5000, 0x1000);
        }
        gTaskCurrent->func = TitleMenu_State_Unk_ov001_022d85d0;
    }
}
#pragma thumb reset
