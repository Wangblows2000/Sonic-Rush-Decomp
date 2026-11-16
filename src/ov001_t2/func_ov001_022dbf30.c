#include <nitro/types.h>

typedef struct {
    s16 x;
    s16 y;
    u8 pad4[0x28];
} UnkAnim_022dbf30;

typedef struct {
    s32 timer;
    u8 pad4[0xb44 - 4];
    UnkAnim_022dbf30 anims[2];
} UnkWork_022dbf30;

typedef struct {
    u8 pad0[8];
    void (*fn)(void);
    u8 padc[4];
    UnkWork_022dbf30 *work;
} UnkTask_022dbf30;

extern UnkTask_022dbf30 *gTaskCurrent;

void func_02040d14(UnkAnim_022dbf30 *a);
BOOL func_02040cf4(UnkAnim_022dbf30 *a);
void func_ov001_022da450(UnkWork_022dbf30 *w);
void func_02050078(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
void func_ov001_022dbe3c(void);
void func_ov001_022d9e50(UnkWork_022dbf30 *w, s32 x, s32 y);
void func_ov001_022d9e2c(UnkWork_022dbf30 *w, s32 x, s32 y);

#pragma thumb on
void func_ov001_022dbf30(void) {
    UnkWork_022dbf30 *w = gTaskCurrent->work;
    BOOL done;

    func_02040d14(&w->anims[0]);
    func_02040d14(&w->anims[1]);
    {
        UnkAnim_022dbf30 *a = &w->anims[0];
        func_ov001_022d9e50(w, a->x, a->y);
    }
    {
        UnkAnim_022dbf30 *a = &w->anims[1];
        func_ov001_022d9e2c(w, a->x, a->y);
    }
    done = TRUE;
    if (!func_02040cf4(&w->anims[0])) {
        done = FALSE;
    }
    if (!func_02040cf4(&w->anims[1])) {
        done = FALSE;
    }
    w->timer++;
    if (done) {
        w->timer = 0;
        func_ov001_022da450(w);
        func_02050078(4, 3, 5, 1, 2, 0, 0xe, 0x200010);
        gTaskCurrent->fn = func_ov001_022dbe3c;
    }
}
#pragma thumb reset
