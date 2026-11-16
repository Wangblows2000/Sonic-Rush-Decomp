#include <nitro/types.h>

typedef struct {
    s16 x;
    s16 y;
    u8 pad4[0x28];
} UnkAnim_022dad40;

typedef struct {
    s32 timer;
    s32 unk4;
    u8 pad8[0xb44 - 8];
    UnkAnim_022dad40 anims[6];
} UnkWork_022dad40;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022dad40 *work;
} UnkTask_022dad40;

extern UnkTask_022dad40 *gTaskCurrent;

void func_02040d14(UnkAnim_022dad40 *a);
BOOL func_02040cf4(UnkAnim_022dad40 *a);
void func_ov001_022d9e50(UnkWork_022dad40 *w, s32 x, s32 y);
void func_ov001_022d9e2c(UnkWork_022dad40 *w, s32 x, s32 y);
void func_ov001_022d9d98(UnkWork_022dad40 *w, s32 i, s32 x, s32 y);
void func_ov001_022d9d64(UnkWork_022dad40 *w, s32 i, s32 x, s32 y);
void func_ov001_022da154(UnkWork_022dad40 *w, s32 a);
void func_ov001_022da244(UnkWork_022dad40 *w, s32 a);
void func_ov001_022da1cc(UnkWork_022dad40 *w, s32 a);
void func_ov001_022da374(UnkWork_022dad40 *w);
void func_ov001_022da33c(UnkWork_022dad40 *w);
void func_02040ec0(UnkAnim_022dad40 *a, s32 id);
void func_02040e80(UnkAnim_022dad40 *a);
void func_ov001_022dbf30(void);

#pragma thumb on
void func_ov001_022dad40(void) {
    UnkWork_022dad40 *w;
    BOOL done;
    UnkAnim_022dad40 *a;

    w = gTaskCurrent->work;
    func_02040d14(&w->anims[0]);
    func_02040d14(&w->anims[1]);
    func_02040d14(&w->anims[2]);
    func_02040d14(&w->anims[3]);
    func_02040d14(&w->anims[4]);
    func_02040d14(&w->anims[5]);
    done = TRUE;
    if (!func_02040cf4(&w->anims[0])) done = FALSE;
    if (!func_02040cf4(&w->anims[1])) done = FALSE;
    if (!func_02040cf4(&w->anims[2])) done = FALSE;
    if (!func_02040cf4(&w->anims[3])) done = FALSE;
    if (!func_02040cf4(&w->anims[4])) done = FALSE;
    if (!func_02040cf4(&w->anims[5])) done = FALSE;
    a = &w->anims[0];
    func_ov001_022d9e50(w, a->x, a->y);
    a = &w->anims[1];
    func_ov001_022d9e2c(w, a->x, a->y);
    a = &w->anims[2];
    func_ov001_022d9d98(w, 0, a->x, a->y);
    a = &w->anims[3];
    func_ov001_022d9d98(w, 1, a->x, a->y);
    a = &w->anims[4];
    func_ov001_022d9d64(w, 0, a->x, a->y);
    a = &w->anims[5];
    func_ov001_022d9d64(w, 1, a->x, a->y);
    w->timer++;
    if (done) {
        w->timer = 0;
        w->unk4 = 0;
        func_ov001_022da374(w);
        func_ov001_022da33c(w);
        func_02040ec0(&w->anims[0], 0x1e);
        func_02040ec0(&w->anims[1], 0x1f);
        func_02040e80(&w->anims[0]);
        func_02040e80(&w->anims[1]);
        gTaskCurrent->func = func_ov001_022dbf30;
    }
}
#pragma thumb reset
