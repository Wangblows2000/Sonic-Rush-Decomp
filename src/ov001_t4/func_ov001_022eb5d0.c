#include <nitro/types.h>

typedef struct {
    s16 x;
    s16 y;
    s32 sx;
    s32 sy;
    u8 padc[0x20];
} UnkAnim_022eb5d0;

typedef struct {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x40];
} UnkSprite_022eb5d0;

typedef struct {
    u8 pad0[8];
    s32 unk8;
    u8 padc[0x1c];
} UnkItem_022eb5d0;

typedef struct {
    u32 unk0;
    u32 unk4;
    UnkItem_022eb5d0 items[5];
    s32 unkd0;
    s32 unkd4;
    s32 unkd8;
    s32 unkdc;
    s32 unke0;
    u8 pade4[0x7c8 - 0xe4];
    UnkSprite_022eb5d0 sprites[11];
    u8 padae0[0xb5c - 0xae0];
    UnkAnim_022eb5d0 anims[12];
} UnkWork_022eb5d0;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022eb5d0 *work;
} UnkTask_022eb5d0;

extern UnkTask_022eb5d0 *data_022b4574;

void func_02040d14(UnkAnim_022eb5d0 *a);
BOOL func_02040cf4(UnkAnim_022eb5d0 *a);
void func_ov001_022ea360(UnkWork_022eb5d0 *w, s32 i, s32 v);
void func_ov001_022e9fe4(UnkWork_022eb5d0 *w, s32 i);
void func_ov001_022e9fa8(UnkWork_022eb5d0 *w, s32 i);
void func_ov001_022ea2f0(UnkWork_022eb5d0 *w);
void func_0203344c(void *p, s32 a, s32 b);
void func_02032d2c(void *p);
void func_02032480(void *p, s32 sx, s32 sy, s32 d);
BOOL func_ov001_022e9ebc(UnkWork_022eb5d0 *w);
BOOL func_ov001_022e9e30(UnkWork_022eb5d0 *w);
BOOL func_ov001_022e9df0(UnkWork_022eb5d0 *w);
void func_ov001_022ebc3c(s32 a);
void func_ov001_022eba70(s32 a);

#pragma thumb on
void func_ov001_022eb5d0(void) {
    UnkWork_022eb5d0 *w = data_022b4574->work;
    UnkAnim_022eb5d0 *a;
    s32 i;
    BOOL done;
    UnkAnim_022eb5d0 *an;
    UnkItem_022eb5d0 *it;

    func_02040d14(&w->anims[0]);
    func_02040d14(&w->anims[1]);
    func_02040d14(&w->anims[2]);
    func_02040d14(&w->anims[3]);
    func_02040d14(&w->anims[4]);
    func_02040d14(&w->anims[5]);
    func_02040d14(&w->anims[6]);
    func_02040d14(&w->anims[7]);
    func_02040d14(&w->anims[8]);
    func_02040d14(&w->anims[9]);
    func_02040d14(&w->anims[10]);
    func_02040d14(&w->anims[11]);
    i = 0;
    it = w->items;
    an = w->anims;
    for (; i < 5; i++) {
        func_ov001_022ea360(w, i, an->x);
        it->unk8 = 8;
        it->unk8 += i * 0x18;
        func_ov001_022e9fe4(w, i);
        func_ov001_022e9fa8(w, i);
        it++;
        an++;
    }
    func_ov001_022ea2f0(w);
    a = &w->anims[6];
    if (w->unkd0 >= 14) {
        w->sprites[10].x = a->x;
        w->sprites[10].y = a->y;
        func_0203344c(&w->sprites[10], 0, 0);
        func_02032d2c(&w->sprites[10]);
    } else {
        w->sprites[5].x = a->x;
        w->sprites[5].y = a->y;
        func_0203344c(&w->sprites[5], 0, 0);
        func_02032d2c(&w->sprites[5]);
    }
    if (w->unk4 <= 1) {
        a = &w->anims[7];
        w->sprites[0].x = a->x;
        w->sprites[0].y = a->y;
        func_0203344c(&w->sprites[0], 0, 0);
        func_02032d2c(&w->sprites[0]);
    }
    a = &w->anims[8];
    w->sprites[1].x = a->x;
    w->sprites[1].y = a->y;
    func_0203344c(&w->sprites[1], 0, 0);
    func_02032d2c(&w->sprites[1]);
    a = &w->anims[9];
    w->sprites[2].x = a->x;
    w->sprites[2].y = a->y;
    func_0203344c(&w->sprites[2], 0, 0);
    func_02032d2c(&w->sprites[2]);
    a = &w->anims[5];
    if (w->unkd0 >= 14) {
        w->sprites[9].x = a->x;
        w->sprites[9].y = a->y;
        func_0203344c(&w->sprites[9], 0, 0);
        if (a->sx == 0x1000 && a->sy == 0x1000) {
            func_02032d2c(&w->sprites[9]);
        } else {
            func_02032480(&w->sprites[9], a->sx, a->sy, 0);
        }
    } else {
        w->sprites[3].x = a->x;
        w->sprites[3].y = a->y;
        func_0203344c(&w->sprites[3], 0, 0);
        if (a->sx == 0x1000 && a->sy == 0x1000) {
            func_02032d2c(&w->sprites[3]);
        } else {
            func_02032480(&w->sprites[3], a->sx, a->sy, 0);
        }
    }
    a = &w->anims[10];
    w->sprites[8].x = a->x;
    w->sprites[8].y = a->y;
    func_0203344c(&w->sprites[8], 0, 0);
    func_02032d2c(&w->sprites[8]);
    a = &w->anims[11];
    w->sprites[4].x = a->x;
    w->sprites[4].y = a->y;
    func_0203344c(&w->sprites[4], 0, 0);
    func_02032d2c(&w->sprites[4]);
    w->unk0++;
    done = TRUE;
    if (!func_02040cf4(&w->anims[0])) done = FALSE;
    if (!func_02040cf4(&w->anims[1])) done = FALSE;
    if (!func_02040cf4(&w->anims[2])) done = FALSE;
    if (!func_02040cf4(&w->anims[3])) done = FALSE;
    if (!func_02040cf4(&w->anims[4])) done = FALSE;
    if (!func_02040cf4(&w->anims[5])) done = FALSE;
    if (!func_02040cf4(&w->anims[6])) done = FALSE;
    if (!func_02040cf4(&w->anims[7])) done = FALSE;
    if (!func_02040cf4(&w->anims[8])) done = FALSE;
    if (!func_02040cf4(&w->anims[9])) done = FALSE;
    if (!func_02040cf4(&w->anims[10])) done = FALSE;
    if (!func_02040cf4(&w->anims[11])) done = FALSE;
    if (done) {
        w->unkdc = 0;
        func_ov001_022ebc3c(0);
    }
}
#pragma thumb reset
