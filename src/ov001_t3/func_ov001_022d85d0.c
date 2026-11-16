#include <nitro/types.h>

typedef struct {
    s16 x;
    s16 y;
    u8 pad4[0x28];
} UnkAnim_022d85d0;

typedef struct {
    u8 pad0[0x258];
    s32 unk258;
} UnkObj_022d85d0;

typedef struct {
    s32 id;
    s32 unk4;
    UnkAnim_022d85d0 anim;
    s32 arg;
    void (*cb)(UnkObj_022d85d0 *obj, s32 a, s32 b);
} UnkItem_022d85d0;

typedef struct {
    u8 pad0[4];
    s32 sel;
    s32 unk8;
    u8 padc[0x12 - 0xc];
    s16 y;
    u8 pad14[2];
    s16 dy;
    u8 pad18[0x20 - 0x18];
    s32 unk20;
    UnkItem_022d85d0 items[6];
    u8 pad180[0x1a4 - 0x18c];
    UnkObj_022d85d0 *obj;
} UnkWork_022d85d0;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022d85d0 *work;
} UnkTask_022d85d0;

extern UnkTask_022d85d0 *gTaskCurrent;

void KeyframeAnim_Update(UnkAnim_022d85d0 *a);
BOOL KeyframeAnim_IsFinished(UnkAnim_022d85d0 *a);
void func_ov001_022d7b7c(s32 x, s32 y, s32 a, s32 b);
void func_ov001_022d7ac4(s32 idx, s16 x, s16 y);
void func_ov001_022d7c04(s32 id, s32 x, s32 y, s32 mode, s32 e);
void Task_Destroy(UnkTask_022d85d0 *t);

#pragma thumb on
void func_ov001_022d85d0(void) {
    UnkWork_022d85d0 *w;
    BOOL done;
    s16 y;
    s32 i;
    UnkItem_022d85d0 *it;

    w = gTaskCurrent->work;
    done = TRUE;
    y = w->y;
    for (i = 0, it = w->items; i < 6; it++, i++) {
        if (it->id == 0x14) continue;
        KeyframeAnim_Update(&it->anim);
        if (!KeyframeAnim_IsFinished(&it->anim)) done = FALSE;
        if (i == 4) {
            func_ov001_022d7b7c(it->anim.x, it->anim.y, 0, 0);
        } else if (i == 5) {
            func_ov001_022d7ac4(it->id, it->anim.x, it->anim.y);
        } else {
            if (w->sel != i) {
                func_ov001_022d7c04(it->id, it->anim.x, y, 0, 0);
            } else {
                func_ov001_022d7c04(it->id, it->anim.x, y, 3, 0);
            }
            y += w->dy;
        }
    }
    if (done == TRUE) {
        it = &w->items[w->sel];
        if (it->cb != NULL) {
            it->cb(w->obj, w->unk20, it->arg);
        }
        Task_Destroy(gTaskCurrent);
    }
}
#pragma thumb reset
