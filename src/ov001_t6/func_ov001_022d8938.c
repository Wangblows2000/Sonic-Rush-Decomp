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
    void *res190;
    void *res194[4];
    void *sw;
} UnkWork_022d8798;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022d8798 *work;
} UnkTask_022d8798;

typedef struct {
    u8 pad0[4];
    u16 trg;
    u8 pad6[2];
    u16 rep;
} UnkPad_022d8938;

typedef struct {
    u8 pad0[8];
    u16 x;
    u16 y;
    u8 padc[4];
    u32 flags;
} UnkTouch_022d8938;

extern UnkTask_022d8798 *gTaskCurrent;
extern UnkPad_022d8938 gPad;
extern UnkTouch_022d8938 gTouchState;

void func_ov001_022d7b7c(s32 x, s32 y, s32 a, s32 b);
void func_ov001_022d7ac4(s32 id, s32 x, s32 y);
void func_ov001_022d7c04(s32 idx, s16 x, s16 y, s32 mode, u32 t);
void SysSound_PlaySE(s32 a);
void func_ov001_022d7dbc(void *p, void *sw);
void func_ov001_022d75dc(void *sw, void *a, void *b);
s32 func_ov001_022d7a84(u32 x, u32 y, s32 y0);
BOOL func_ov001_022d7a60(u32 x, u32 y);
void func_ov001_022d7968(s32 a, s32 b);
void func_ov001_022d78f4(s32 a, s32 b);
void func_ov001_022d8798(void);
void func_ov001_022d85d0(void);
void func_ov001_022d86b0(void);

#pragma thumb on
#pragma opt_common_subs off
void func_ov001_022d8938(void) {
    UnkWork_022d8798 *w;
    s16 y;
    s32 i;
    UnkItem_022d8798 *it;
    u32 tx;
    u32 ty;
    u16 key;

    w = gTaskCurrent->work;
    y = w->y0;

    i = 0;
    it = w->items;
    for (; i < 6; i++) {
        if (it->id != 0x14) {
            if (i == 4) {
                func_ov001_022d7b7c(it->x, it->y, 0, 0);
            } else if (i == 5) {
                func_ov001_022d7ac4(it->id, it->x, it->y);
            } else {
                if (w->sel == i) {
                    func_ov001_022d7c04(it->id, it->x, y, 1, w->timer);
                } else {
                    func_ov001_022d7c04(it->id, it->x, y, 0, 0);
                }
                y += w->dy;
            }
        }
        it++;
    }
    key = gPad.rep;
    if (key & 0x40) {
        do {
            w->sel--;
            if (w->sel < 0) {
                w->sel = 3;
            }
        } while (w->items[w->sel].id == 0x14);
        w->timer = 0;
        SysSound_PlaySE(4);
    } else if (key & 0x80) {
        do {
            w->sel++;
            if (w->sel >= 4) {
                w->sel = 0;
            }
        } while (w->items[w->sel].id == 0x14);
        w->timer = 0;
        SysSound_PlaySE(4);
    }
    key = gPad.trg;
    if (key & 1) {
        w->timer = 0;
        gTaskCurrent->func = func_ov001_022d8798;
        SysSound_PlaySE(0);
    } else if (key & 2) {
        w->sel = 4;
        w->timer = 0;
        func_ov001_022d7dbc(w->obj10, w->sw);
        func_ov001_022d75dc(w->sw, w->res18c, w->res190);
        SysSound_PlaySE(1);
        gTaskCurrent->func = func_ov001_022d85d0;
        if (w->items[4].flags & 1) {
            func_ov001_022d7968(8, 0x14);
        }
        if (w->items[4].flags & 2) {
            func_ov001_022d78f4(8, 0x14);
        }
    } else if (gTouchState.flags & 0x40) {
        tx = gTouchState.x;
        ty = gTouchState.y;
        if (tx < 0x100 && ty < 0xc0) {
            i = func_ov001_022d7a84(tx, ty, w->y0);
            if (i >= 0) {
                if (w->items[i].id != 0x14) {
                    w->sel = i;
                    w->timer = 0;
                    gTaskCurrent->func = func_ov001_022d8798;
                    SysSound_PlaySE(0);
                }
            } else if (func_ov001_022d7a60(tx, ty)) {
                w->sel = 4;
                w->timer = 0;
                gTaskCurrent->func = func_ov001_022d86b0;
                SysSound_PlaySE(1);
            }
        }
    }
    w->timer++;
}
#pragma thumb reset
