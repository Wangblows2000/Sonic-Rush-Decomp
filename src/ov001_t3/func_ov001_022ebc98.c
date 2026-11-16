#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
} UnkAnim_022ebc98;

typedef struct {
    s32 a;
    s32 b;
    u8 pad8[0x20];
} UnkItem_022ebc98;

typedef struct {
    s32 unk0;
    u8 pad4[4];
    UnkItem_022ebc98 items[5];
    s32 unkd0;
    u8 padd4[0xb5c - 0xd4];
    UnkAnim_022ebc98 anims[12];
} UnkWork_022ebc98;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022ebc98 *work;
} UnkTask_022ebc98;

extern UnkTask_022ebc98 *gTaskCurrent;

void func_ov001_022e9708(UnkWork_022ebc98 *w, s32 a, s32 *x, s32 *y);
void KeyframeAnim_SetAnim(UnkAnim_022ebc98 *a, s32 id);
void func_ov001_022ea850(UnkWork_022ebc98 *w);
void func_ov001_022ea7c4(UnkWork_022ebc98 *w);
void func_ov001_022ea6e4(UnkWork_022ebc98 *w);
void func_ov001_022ea5a8(UnkWork_022ebc98 *w, s32 x);
void func_ov001_022ea504(UnkWork_022ebc98 *w, s32 x, s32 y);
void func_ov001_022ea41c(UnkWork_022ebc98 *w);
void func_ov001_022ea37c(UnkWork_022ebc98 *w);
void func_ov001_022e9c24(s32 x, s32 y, UnkItem_022ebc98 *items);
void func_ov001_022ea2b0(UnkWork_022ebc98 *w);
void func_ov001_022ea2f0(UnkWork_022ebc98 *w);
void TimeAttackMenu_State_Enter(void);

#pragma thumb on
void func_ov001_022ebc98(UnkTask_022ebc98 *t) {
    UnkWork_022ebc98 *w;
    s32 x;
    s32 y;
    s32 i;

    if (t == NULL) {
        t = gTaskCurrent;
    }
    w = t->work;
    w->unk0 = 0;
    func_ov001_022e9708(w, w->unkd0, &x, &y);
    KeyframeAnim_SetAnim(&w->anims[0], 0);
    KeyframeAnim_SetAnim(&w->anims[1], 1);
    KeyframeAnim_SetAnim(&w->anims[2], 2);
    KeyframeAnim_SetAnim(&w->anims[3], 3);
    KeyframeAnim_SetAnim(&w->anims[4], 4);
    KeyframeAnim_SetAnim(&w->anims[5], 5);
    KeyframeAnim_SetAnim(&w->anims[6], 6);
    KeyframeAnim_SetAnim(&w->anims[7], 7);
    KeyframeAnim_SetAnim(&w->anims[8], 8);
    KeyframeAnim_SetAnim(&w->anims[9], 9);
    KeyframeAnim_SetAnim(&w->anims[10], 10);
    KeyframeAnim_SetAnim(&w->anims[11], 12);
    func_ov001_022ea850(w);
    func_ov001_022ea7c4(w);
    func_ov001_022ea6e4(w);
    func_ov001_022ea5a8(w, x);
    func_ov001_022ea504(w, x, y);
    func_ov001_022ea41c(w);
    func_ov001_022ea37c(w);
    func_ov001_022e9c24(x, y, w->items);
    for (i = 0; i < 5; i++) {
        w->items[i].a = 0x100;
        w->items[i].b = 0x100;
    }
    func_ov001_022ea2b0(w);
    func_ov001_022ea2f0(w);
    t->func = TimeAttackMenu_State_Enter;
}
#pragma thumb reset
