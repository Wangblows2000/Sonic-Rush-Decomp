#include <nitro/types.h>

typedef struct {
    s16 x;
    s16 y;
    u8 pad4[0x28];
} UnkAnim_022dbcd8;

typedef struct {
    s32 timer;
    u8 pad4[0x1c - 4];
    s32 unk1c;
    u8 pad20[0xb44 - 0x20];
    UnkAnim_022dbcd8 anims[14];
} UnkWork_022dbcd8;

typedef struct {
    u8 pad0[8];
    void (*func)(void);
    u8 padc[4];
    UnkWork_022dbcd8 *work;
} UnkTask_022dbcd8;

extern UnkTask_022dbcd8 *gTaskCurrent;

void func_02040d14(UnkAnim_022dbcd8 *a);
BOOL func_02040cf4(UnkAnim_022dbcd8 *a);
void func_ov001_022d9e50(UnkWork_022dbcd8 *w, s32 x, s32 y);
void func_ov001_022d9e2c(UnkWork_022dbcd8 *w, s32 x, s32 y);
void func_ov001_022da374(UnkWork_022dbcd8 *w);
void func_ov001_022da33c(UnkWork_022dbcd8 *w);
void func_ov001_022d9f6c(UnkWork_022dbcd8 *w);
void func_ov001_022d9f2c(UnkWork_022dbcd8 *w);
void func_02040ec0(UnkAnim_022dbcd8 *a, s32 id);
void func_ov001_022dbbd4(void);

#pragma thumb on
void func_ov001_022dbcd8(void) {
    UnkWork_022dbcd8 *w;
    BOOL done;
    UnkAnim_022dbcd8 *a;
    s32 i;

    w = gTaskCurrent->work;
    func_02040d14(&w->anims[0]);
    func_02040d14(&w->anims[1]);
    a = &w->anims[0];
    func_ov001_022d9e50(w, a->x, a->y);
    a = &w->anims[1];
    func_ov001_022d9e2c(w, a->x, a->y);
    done = TRUE;
    if (!func_02040cf4(&w->anims[0])) done = FALSE;
    if (!func_02040cf4(&w->anims[1])) done = FALSE;
    w->timer++;
    if (done) {
        w->timer = 0;
        func_ov001_022da374(w);
        func_ov001_022da33c(w);
        func_ov001_022d9f6c(w);
        func_ov001_022d9f2c(w);
        func_02040ec0(&w->anims[0], 0);
        func_02040ec0(&w->anims[1], 1);
        for (i = 0; i < 8; i++) {
            func_02040ec0(&w->anims[6 + i], i + 6);
        }
        w->unk1c = 0;
        gTaskCurrent->func = func_ov001_022dbbd4;
    }
}
#pragma thumb reset
