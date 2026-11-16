#pragma opt_common_subs off
#include <nitro/types.h>

typedef struct {
    s16 x;
    s16 y;
    u8 pad4[0x30];
} UnkAnim_022d86b0;

typedef struct {
    s32 id;
    u32 flags;
    UnkAnim_022d86b0 anim;
} UnkItem_022d86b0;

typedef struct UnkObj_022d86b0 UnkObj_022d86b0;

typedef struct {
    u8 pad0[4];
    s32 sel;
    u32 timer;
    u8 padc[4];
    u8 unk10[2];
    s16 y;
    u8 pad14[2];
    s16 dy;
    u8 pad18[0x24 - 0x18];
    UnkItem_022d86b0 items[6];
    s32 a;
    s32 b;
    u8 pad194[0x1a4 - 0x194];
    UnkObj_022d86b0 *obj;
} UnkWork_022d86b0;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022d86b0 *work;
} UnkTask_022d86b0;

extern UnkTask_022d86b0 *gTaskCurrent;

void func_ov001_022d7b7c(s32 x, s32 y, s32 a, s32 b);
void func_ov001_022d7ac4(s32 idx, s16 x, s16 y);
void func_ov001_022d7c04(s32 id, s32 x, s32 y, s32 mode, s32 e);
void func_ov001_022d7968(s32 a, s32 b);
void func_ov001_022d78f4(s32 a, s32 b);
void func_ov001_022d7dbc(void *p, UnkObj_022d86b0 *obj);
void func_ov001_022d75dc(UnkObj_022d86b0 *obj, s32 a, s32 b);
void func_ov001_022d85d0(void);

#pragma thumb on
void func_ov001_022d86b0(void) {
    UnkWork_022d86b0 *w;
    u32 t;
    s16 y;
    s32 i;
    UnkItem_022d86b0 *it;

    w = gTaskCurrent->work;
    t = w->timer;
    y = w->y;
    for (i = 0, it = w->items; i < 6; it++, i++) {
        if (it->id == 0x14) continue;
        if (i == 4) {
            func_ov001_022d7b7c(it->anim.x, it->anim.y, 2, t);
        } else if (i == 5) {
            func_ov001_022d7ac4(it->id, it->anim.x, it->anim.y);
        } else {
            func_ov001_022d7c04(it->id, it->anim.x, y, 0, 0);
            y += w->dy;
        }
    }
    w->timer++;
    if (t >= 8) {
        it = &w->items[w->sel];
        if (it->flags & 1) {
            func_ov001_022d7968(8, 0x14);
        }
        if (it->flags & 2) {
            func_ov001_022d78f4(8, 0x14);
        }
        func_ov001_022d7dbc(w->unk10, w->obj);
        func_ov001_022d75dc(w->obj, w->a, w->b);
        gTaskCurrent->func = func_ov001_022d85d0;
    }
}
#pragma thumb reset
