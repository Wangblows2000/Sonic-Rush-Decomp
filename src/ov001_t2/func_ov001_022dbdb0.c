#include <nitro/types.h>

typedef struct {
    s16 x;
    s16 y;
    u8 pad4[0x28];
} UnkAnim_022dbdb0;

typedef struct {
    s32 timer;
    u8 pad4[0xb44 - 4];
    UnkAnim_022dbdb0 anims[2];
} UnkWork_022dbdb0;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022dbdb0 *work;
} UnkTask_022dbdb0;

extern UnkTask_022dbdb0 *data_022b4574;

void func_02040d14(UnkAnim_022dbdb0 *a);
BOOL func_02040cf4(UnkAnim_022dbdb0 *a);
void func_020371c8(UnkTask_022dbdb0 *t);
void func_ov001_022d9e50(UnkWork_022dbdb0 *w, s32 x, s32 y);
void func_ov001_022d9e2c(UnkWork_022dbdb0 *w, s32 x, s32 y);

#pragma thumb on
void func_ov001_022dbdb0(void) {
    UnkWork_022dbdb0 *w = data_022b4574->work;
    BOOL done;

    func_02040d14(&w->anims[0]);
    func_02040d14(&w->anims[1]);
    done = TRUE;
    if (!func_02040cf4(&w->anims[0])) {
        done = FALSE;
    }
    if (!func_02040cf4(&w->anims[1])) {
        done = FALSE;
    }
    if (done) {
        w->timer = 0;
        func_020371c8(data_022b4574);
        return;
    }
    {
        UnkAnim_022dbdb0 *a = &w->anims[0];
        func_ov001_022d9e50(w, a->x, a->y);
    }
    {
        UnkAnim_022dbdb0 *a = &w->anims[1];
        func_ov001_022d9e2c(w, a->x, a->y);
    }
    w->timer++;
}
#pragma thumb reset
