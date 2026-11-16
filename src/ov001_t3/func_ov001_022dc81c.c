#include <nitro/types.h>

typedef struct {
    s16 x;
    s16 y;
    u8 pad4[0x28];
} UnkAnim_022dc81c;

typedef struct {
    s32 timer;
    s32 unk4;
    u8 pad8[0xb44 - 8];
    UnkAnim_022dc81c anims[6];
} UnkWork_022dc81c;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022dc81c *work;
} UnkTask_022dc81c;

extern UnkTask_022dc81c *data_022b4574;

void func_02040d14(UnkAnim_022dc81c *a);
BOOL func_02040cf4(UnkAnim_022dc81c *a);
void func_ov001_022d9e50(UnkWork_022dc81c *w, s32 x, s32 y);
void func_ov001_022d9e2c(UnkWork_022dc81c *w, s32 x, s32 y);
void func_ov001_022d9d98(UnkWork_022dc81c *w, s32 i, s32 x, s32 y);
void func_ov001_022d9d64(UnkWork_022dc81c *w, s32 i, s32 x, s32 y);
void func_ov001_022da154(UnkWork_022dc81c *w, s32 a);
void func_ov001_022da244(UnkWork_022dc81c *w, s32 a);
void func_ov001_022da1cc(UnkWork_022dc81c *w, s32 a);
void func_020511d0(s32 a);
void func_ov001_022dc734(void);

#pragma thumb on
void func_ov001_022dc81c(void) {
    UnkWork_022dc81c *w;
    BOOL done;
    UnkAnim_022dc81c *a;

    w = data_022b4574->work;
    func_02040d14(&w->anims[0]);
    func_02040d14(&w->anims[1]);
    func_02040d14(&w->anims[2]);
    func_02040d14(&w->anims[3]);
    func_02040d14(&w->anims[4]);
    func_02040d14(&w->anims[5]);
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
    done = TRUE;
    if (!func_02040cf4(&w->anims[0])) done = FALSE;
    if (!func_02040cf4(&w->anims[1])) done = FALSE;
    if (!func_02040cf4(&w->anims[2])) done = FALSE;
    if (!func_02040cf4(&w->anims[3])) done = FALSE;
    if (!func_02040cf4(&w->anims[4])) done = FALSE;
    if (!func_02040cf4(&w->anims[5])) done = FALSE;
    w->timer++;
    if (done) {
        w->timer = 0;
        w->unk4 = 0;
        func_ov001_022da154(w, 0);
        func_ov001_022da244(w, 1);
        func_ov001_022da1cc(w, 1);
        func_020511d0(0);
        data_022b4574->func = func_ov001_022dc734;
    }
}
#pragma thumb reset
