#include <nitro/types.h>

typedef struct {
    s16 x;
    s16 y;
    u8 pad4[0x30];
} UnkAnim_022d8b54;

typedef struct {
    s32 id;
    s32 unk4;
    UnkAnim_022d8b54 anim;
} UnkItem_022d8b54;

typedef struct {
    u8 pad0[0x258];
    s32 unk258;
} UnkObj_022d8b54;

typedef struct {
    u8 pad0[4];
    s32 sel;
    s32 unk8;
    u8 padc[0x12 - 0xc];
    s16 y;
    u8 pad14[2];
    s16 dy;
    u8 pad18[0x24 - 0x18];
    UnkItem_022d8b54 items[6];
    u8 pad180[0x1a4 - 0x18c];
    UnkObj_022d8b54 *obj;
} UnkWork_022d8b54;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022d8b54 *work;
} UnkTask_022d8b54;

extern UnkTask_022d8b54 *data_022b4574;

void func_02040d14(UnkAnim_022d8b54 *a);
BOOL func_02040cf4(UnkAnim_022d8b54 *a);
void func_ov001_022d7b7c(s32 x, s32 y, s32 a, s32 b);
void func_ov001_022d7ac4(s32 idx, s16 x, s16 y);
void func_ov001_022d7c04(s32 id, s32 x, s32 y, s32 mode, s32 e);
void func_ov001_022d8938(void);

#pragma thumb on
void func_ov001_022d8b54(void) {
    UnkWork_022d8b54 *w;
    BOOL done;
    s16 y;
    s32 i;
    UnkItem_022d8b54 *it;

    w = data_022b4574->work;
    done = TRUE;
    y = w->y;
    for (i = 0, it = w->items; i < 6; it++, i++) {
        if (it->id == 0x14) continue;
        func_02040d14(&it->anim);
        if (!func_02040cf4(&it->anim)) done = FALSE;
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
    if (done == TRUE && w->obj->unk258 != 0) {
        w->unk8 = 7;
        data_022b4574->func = func_ov001_022d8938;
    }
}
#pragma thumb reset
